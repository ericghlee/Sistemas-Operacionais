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

struct cyclist {
	int id;
	int speed;
	int position;
	int lane;
	int lap;
	bool half;
};

// Seu codigo deve possuir um vetor compartilhado “pista” que tem um tamanho igual a d
int length = 250;
int n_cyclists = 1;
int final_lap = 160;
int lap = 0;
int idCounter = 1;
struct cyclist **pista;
sem_t** sem_pista;
pthread_mutex_t lock;
struct cyclist *empty;

int mod(int a, int b) {
	if(b < 0)
    	return mod(a, -b);   
   	int ret = a % b;
   	if(ret < 0)
    	ret+=b;

   return ret;
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
	if (pista[c->position+1][c->lane].id == 0) {
		for (int j = c->lane; j < 9; j++) {
			if (pista[c->position+1][j].id == 0) {
				return j;
			}
		}

		return -1;
	}

	return 0;
}

void moveCyclistPosition(struct cyclist *c, int position, int lane) {
	sem_wait(&sem_pista[position][lane]);
	sem_wait(&sem_pista[c->position][c->lane]);

	pista[c->position][c->lane] = *empty;
	pista[position][lane] = *c;
	c->position = position;
	c->lane = lane;

	sem_post(&sem_pista[c->position][c->lane]);
	sem_post(&sem_pista[position][lane]);
}

void setCyclistPosition(struct cyclist *c, int position, int lane) {
	sem_wait(&sem_pista[position][lane]);

	pista[position][lane] = *c;
	c->position = position;
	c->lane = lane;

	sem_post(&sem_pista[position][lane]);
}

void move(struct cyclist *c) {
// 	A simulacao do seu codigo deve simular a corrida em intervalos de 60ms ate as duas ultimas voltas.
//  A partir das duas ultimas voltas, caso algum ciclista tenha sido sorteado para pedalar a 90Km/h, 
//  a simulacao deve passar a simular a corrida em intervalos de 20ms.
	int move = canMove(c);
	if (move > 0) {
		if (c->lap == length - 3) {
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
	while(c->lap < 160) {
		if (c->position == 250) {
			c->position = 0;
			c->lap++;

			if (c->lap < 158) {
				getNewSpeed(*c);
			}
		}

		if ((c->lap + 1) % 15 == 0) {
		// Assim como no mundo real, ciclistas podem “quebrar” durante a prova e desistirem.
		// Considere que a cada vez que um ciclista completa multiplos de 15 voltas.
			
		}
		move(c);
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
			position = mod(position - 1, 249);
		} else {
			lane++;
		}
	}

	setCyclistPosition(c, position, lane);
}

void *prepareAndStart(void *arg) {
	struct cyclist *c = (struct cyclist* ) malloc(sizeof(struct cyclist));	
	c->speed = 30;
	c->lap = 0;
	c->id = getCyclistId();

	setStartPosition(c);
	// tenque ter uma barreira aqui pra todos terem posição definidas
	start(c);

	return NULL;
}

// Seu simulador deve criar n threads “ciclista” iguais.
void createThreads(int n, struct cyclist *cyclists[]) {
	pthread_t cthreads[n];

	for (int i = 0; i < n; i++) {

		if (pthread_create( &cthreads[i], NULL, prepareAndStart, cyclists[i]) ) {
			printf("Erro ao criar a thread");
			abort();
		}
	}
}

void prepareSemPista() {
	sem_pista = (sem_t**)malloc (length * sizeof(sem_t*));
	for (int i = 0; i < length; ++i) {
	    sem_pista[i] = (sem_t*) malloc (10 * sizeof(sem_t));
	}

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < 10; j++) {
			sem_init(&sem_pista[i][j], 0, 1);
		}
	}
}

int main(int argc, char **argv) {
	empty = malloc(sizeof(struct cyclist));
	empty->id = 0;
	prepareSemPista();

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < 10; j++) {
			pista[i][j] = *empty;
		}
	}

	if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return EXIT_SUCCESS;
    }

    struct cyclist *cyclists[n_cyclists]; 
    srand(time(NULL)); 

    createThreads(n_cyclists, cyclists);

	pthread_exit(NULL);
	pthread_mutex_destroy(&lock);

    return EXIT_SUCCESS;
}

// // testa start position
// int main(int argc, char **argv) {
// 	empty = malloc(sizeof(struct cyclist));
// 	empty->id = 0;
// 	prepareSemPista();

//     pista = (struct cyclist**)malloc (length * sizeof(struct cyclist*));
// 	for (int i = 0; i < length; i++) {
// 	    pista[i] = (struct cyclist*) malloc (10 * sizeof(struct cyclist));
// 	}

// 	for (int i = 0; i < length; i++) {
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
