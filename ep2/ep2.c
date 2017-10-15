// Eric Gun Ho Lee
// NUSP 7557095

#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "barreira.c"

struct cyclist {
	int id;
	int speed;
	int position;
	int lane;
	int lap;
	int points;
	bool half;
};

// Seu codigo deve possuir um vetor compartilhado “pista” que tem um tamanho igual a d
int d_length;

int n_cyclists;

int v_final_lap;

int lap = 0;
int idCounter = 1;
int cyclist_count;
int point_aux[16];

// Cada posicao do vetor corresponde portanto a 1 metro da pista.
// Em um dado instante de tempo, a posicao i da pista deve possuir os identificadores de todos os ciclistas que estao naquele trecho.
struct cyclist **pista;

struct cyclist **cyclists;

sem_t*** sem_pista;
pthread_mutex_t lock;
struct cyclist *empty;

pthread_barrier_t barrier;

pthread_t *cthreads;

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* rand_string_alloc(size_t size)
{
     char *s = malloc(size + 1);
     if (s) {
         rand_string(s, size);
     }
     return s;
}

int mod(int a, int b) {
	if(b < 0)
    	return mod(a, -b);   
   	int ret = a % b;
   	if(ret < 0)
    	ret+=b;

   return ret;
}

void info(struct cyclist *c) {
	printf("id:%d, pos:%d, lane:%d, lap:%d, points: %d \n", c->id, c->position, c->lane, c->lap, c->points);
}

void getNewSpeed(struct cyclist c) {
	int r = (rand() % 100) + 1;
		
	// Caso a volta anterior tenha sido feita a 30Km/h, o sorteio e feito com 70%
	// de chance de escolher 60Km/h e 30% de chance de escolher 30Km/h.
	if (c.speed == 30) {
		if (r < 70) {
			c.speed = 60;
		}
	}
	
	// Caso a volta anterior tenha sido feita a 60Km/h, o sorteio e feito com 50%
	// de chance de escolher 30Km/h e 50% de chance de escolher 60Km/h.
	else {
		if (r < 50) {
			c.speed = 30;
		}
	}
}

int getCyclistId() {
    pthread_mutex_lock(&lock);

    int id = idCounter;
    idCounter++;

    pthread_mutex_unlock(&lock);

    return id;
}

bool has_blocking(struct cyclist c) {
	return true;
}

int canMove(struct cyclist *c) {
	if (pista[mod(c->position+1, d_length-1)][c->lane].id == 0) {
		for (int j = c->lane; j < 9; j++) {
			if (pista[mod(c->position+1, d_length-1)][j].id == 0) {
				// Ultrapassagens podem ser realizadas caso haja espaco em alguma pista mais externa (ultrapassagens so
				// podem ser realizadas usando as pistas externas). Desconsidere a aceleracao necessaria para mudar de velocidade. 
				return j;
			}
		}

		return -1;
	}

	return 0;
}

//  Considere que cada ciclista ocupa exatamente 1 metro da pista.
void moveCyclistPosition(struct cyclist *c, int position, int lane) {
// Como e possıvel perceber, cada posicao do vetor corresponde a uma variavel compartilhada que deve ter seu
// acesso controlado.
// printf("a\n");
    // pthread_mutex_lock(&lock);
	// if (position > d_length) {
		sem_wait(sem_pista[mod(position, d_length-1)][lane]);
	// 	sem_wait(&sem_pista[c->position][c->lane]);
	// } else {
	// 	sem_wait(&sem_pista[c->position][c->lane]);
	// 	sem_wait(&sem_pista[position % (d_length-1)][lane]);
	// }

// 	Cada thread ciclista tem a obrigacao de escrever seu identificador na posicao correta do vetor pista
//  a cada momento em que ele entra em um novo trecho de 1m, e de remover seu identificador da posicaao
//  referente ao trecho que ele acabou de sair.

	pista[c->position][c->lane] = *empty;
	pista[mod(position, d_length-1)][lane] = *c;
	c->position = position;
	c->lane = lane;

	sem_post(sem_pista[mod(position, d_length-1)][lane]);
	// sem_post(&sem_pista[c->position][c->lane]);
    // pthread_mutex_unlock(&lock);
// printf("b\n");
}

void setCyclistPosition(struct cyclist *c, int position, int lane) {
	sem_wait(sem_pista[position][lane]);

	pista[position][lane] = *c;
	c->position = position;
	c->lane = lane;

	sem_post(sem_pista[position][lane]);
}

void move(struct cyclist *c) {
// 	A simulacao do seu codigo deve simular a corrida em intervalos de 60ms ate as duas ultimas voltas.
//  A partir das duas ultimas voltas, caso algum ciclista tenha sido sorteado para pedalar a 90Km/h, 
//  a simulacao deve passar a simular a corrida em intervalos de 20ms.
	// printf("teste \n");
	int move = canMove(c);
	// printf("%d esta movendo para %d \n", c->id, move);
	// if (c->id == 10) {
	// 	info(c);
	// }

	if (move >= 0) {
		if (c->lap == d_length - 3) {
			// intervalos de 20ms
		} else {
			// intervalos de 60ms
			if (c->speed == 60 || c->half == 1) {
				if (move == 0) {
					moveCyclistPosition(c, c->position + 1, c->lane);
				} else {
					moveCyclistPosition(c, c->position + 1, move);
				}
			} else {
				c->half = 1;
			}
		}
	}
}

void start(struct cyclist *c) {
	while(c->lap < v_final_lap) {
		// printf("opaa\n");
		if (c->position == 250) {
			c->position = 0;
			c->lap = c->lap + 1;
			// printf("O ciclista deu id %d volta %d\n", c->id, c->lap);

		// A pontuacao e definida em “sprints” que acontecem a cada 10 voltas, com 5, 3, 2 e 1 ponto(s)
		// sendo atribuıdos as 4 primeiras colocacoes em cada sprint. 
			if ((c->lap + 1) % 10 == 0) {
				int aux = point_aux[(c->lap + 1) / 10];
				if (aux < 4) {
					switch (aux) {
						case 0:
							c->points += 5;
						break;

						case 1:
							c->points += 3;
						break;

						case 2:
							c->points += 2;
						break;

						case 3:
							c->points += 1;
						break;
					}
					point_aux[(c->lap + 1) / 10] += 1;
				}
			}

			if (c->lap < 158) {
				getNewSpeed(*c);
				// printf("nova velocidade de %d eh %d\n", c->id, c->speed);
			}
		}

		// Entretanto, se houverem apenas 5 ciclistas na prova, a probabilidade de quebra para todos deixa de existir.
		if ((c->lap + 1) % 15 == 0 && cyclist_count > 5) {
		// Assim como no mundo real, ciclistas podem “quebrar” durante a prova e desistirem.
		// Considere que a cada vez que um ciclista completa multiplos de 15 voltas ele tem a chance de 1% de quebrar
			int r = (rand() % 100) + 1;
		
			if (r == 1) {
				// Caso algum ciclista quebre, essa informacao deve ser exibida na tela no momento exato em que ele quebrou.
				printf("O ciclista com id=%d quebrou!\n", c->id);
				cyclist_count--;

				// Toda vez que um ciclista quebrar, a thread dele deve ser destruıda.
				pthread_exit(NULL);
				return;
			}
		}

		if (c->lap > lap) {
			lap++;
			// printf("O ciclista %d iniciou a volta %d\n", c->id, lap);
			info(c);
		}

		move(c);
		pthread_barrier_wait(&barrier);
	}
}

void setStartPosition(struct cyclist *c) {
// 10 lado-a-lado imediatamente após a linha de largada/chegada, atrás deles mais 10 lado-a-lado e assim
// sucessivamente até todos terem sua colocação na largada definida.
	int position = 0;
	int lane = 0;

	while (pista[position][lane].speed != 0) {
		if (lane == 9) {
			lane = 0;
			position = mod(position - 1, d_length-1);
		} else {
			lane++;
		}
	}

	printf("ciclista %d vai comecar na pos %d e linha %d\n", c->id, position, lane);
	setCyclistPosition(c, position, lane);
}

void *prepareAndStart(void *arg) {
	struct cyclist *c =  (struct cyclist *) arg;
	c = (struct cyclist* ) malloc(sizeof(struct cyclist));

	c->speed = 30;
	c->lap = 0;
	c->id = getCyclistId();

	setStartPosition(c);
	pthread_barrier_wait(&barrier);

	// Na corrida por pontos, ciclistas iniciam a prova ao mesmo tempo no mesmo lado do velodromo.
	start(c);

	return NULL;
}

// Seu simulador deve criar n threads “ciclista” iguais.
void createThreads(int n, struct cyclist *cyclists[]) {
	cthreads = malloc(n * sizeof(pthread_t *));

	for (int i = 0; i < n; i++) {

		if (pthread_create( &cthreads[i], NULL, prepareAndStart, cyclists[i]) ) {
			printf("Erro ao criar a thread");
			abort();
		}
	}
}

void prepareSemPista() {
	sem_pista = (sem_t***)malloc (d_length * sizeof(sem_t**));
	for (int i = 0; i < d_length; ++i) {
	    sem_pista[i] = (sem_t**) malloc (10 * sizeof(sem_t*));
	}

	for (int i = 0; i < d_length; i++) {
		for (int j = 0; j < 10; j++) {
			// sem_open(&sem_pista[i][j], 0, 1);
			// printf("%d %d\n", i, j);
			char* name = rand_string_alloc(5);
			// sem_unlink(name);
			sem_pista[i][j] = sem_open(name, O_CREAT, 0644, 1);	
		}
	}
}
void preparaPista() {
	empty = malloc(sizeof(struct cyclist));
	empty->id = 0;

    pista = (struct cyclist**)malloc (d_length * sizeof(struct cyclist*));
	for (int i = 0; i < d_length; i++) {
		// A qualquer momento, no maximo, apenas 10 ciclistas podem estar lado a lado em cada ponto da pista.
	    pista[i] = (struct cyclist*) malloc (10 * sizeof(struct cyclist));
	}

	for (int i = 0; i < d_length; i++) {
		for (int j = 0; j < 10; j++) {
			pista[i][j] = *empty;
		}
	}
}

int main(int argc, char **argv) {
// 	Com relacao a entrada, seu simulador deve receber como argumentos nesta
//  ordem os tres numeros inteiros: d n v
	d_length = atoi(argv[1]);
	n_cyclists = atoi(argv[2]);
	v_final_lap = atoi(argv[3]);

	prepareSemPista();
	preparaPista();

	// mutex
	if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return EXIT_SUCCESS;
    }

	cyclists = (struct cyclist**) malloc (n_cyclists * sizeof(struct cyclist*));

    srand(time(NULL)); 

    pthread_barrier_init(&barrier, NULL, n_cyclists);

    createThreads(n_cyclists, cyclists);

	pthread_exit(NULL);
	pthread_mutex_destroy(&lock);

    return EXIT_SUCCESS;
}

// // // testa start position
// int main(int argc, char **argv) {
// 	empty = malloc(sizeof(struct cyclist));
// 	empty->id = 0;
// 	prepareSemPista();

//     pista = (struct cyclist**)malloc (d_length * sizeof(struct cyclist*));
// 	for (int i = 0; i < d_length; i++) {
// 	    pista[i] = (struct cyclist*) malloc (10 * sizeof(struct cyclist));
// 	}

// 	for (int i = 0; i < d_length; i++) {
// 		for (int j = 0; j < 10; j++) {
// 			pista[i][j] = *empty;
// 		}
// 	}

// 	for (int i = 0; i < 12; i++) {
// 		struct cyclist *c = (struct cyclist*) malloc(sizeof(struct cyclist*));
// 		c->speed = 10;
// 		c->id = i+1;
// 		setStartPosition(c);
// 	}

// 	for (int i = 0; i < 10; i++) {
// 		printf("pa %d\n", i);
// 		printf("%d \n", pista[0][i].id);
// 		if (pista[0][i].id == 0) {
// 			printf("DEU RUIM!!! \n");
// 		} 
// 	}

// 	for (int i = 0; i < 2; i++) {
// 		printf("papa %d\n", i);
// 		if (pista[248][i].id == 0) {
// 			printf("DEU RUIM!!!");
// 		} 
// 	}

//     return EXIT_SUCCESS;
// }

// testa semafaros
// int testeS = 0;
// void *testaSemafaro(void *arg) {
// 	int *myid = (int *)arg;
// 	printf("thread %d \n", *myid);
// 	int i = 0;
// 	while (i < 5) {
// 		sem_wait(&sem_pista[0][0]);

// 		int a = testeS;
// 		a = a + 1;
// 		testeS = a;
// 		printf("%d thread: %d \n", testeS, *myid);

// 		sem_post(&sem_pista[0][0]);
// 		// sleep(1);
// 		i++;
// 	}

// 	return NULL;
// }

// int main(int argc, char **argv) {
//     prepareSemPista();

//     printf("oi 3\n");
//     pthread_t cthreads[5];

//     for (int i = 0; i < 5; ++i)
//     {
// 	    if (pthread_create(&cthreads[i], NULL, testaSemafaro, &i) ) {
// 			printf("Erro ao criar a thread");
// 			abort();
// 		}
//     }

//     pthread_exit(NULL);

//     return EXIT_SUCCESS;
// }


// void *testeGetID(void *arg) {
// 	printf("%d \n", getCyclistId());

// 	return NULL;
// }

// int main(int argc, char **argv) {
//     prepareSemPista();

//     pthread_t cthreads[50];

//     for (int i = 0; i < 50; ++i)
//     {
// 	    if (pthread_create(&cthreads[i], NULL, testeGetID, &i) ) {
// 			printf("Erro ao criar a thread");
// 			abort();
// 		}
//     }

//     pthread_exit(NULL);

//     return EXIT_SUCCESS;
// }

// int main(int argc, char **argv) {
// 	d_length = atoi(argv[1]);
// 	n_cyclists = atoi(argv[2]);
// 	v_final_lap = atoi(argv[3]);

// 	printf("d=%d\n", d_length);	
// 	printf("n=%d\n", n_cyclists);	
// 	printf("v=%d\n", v_final_lap);	
// }