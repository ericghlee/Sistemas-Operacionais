#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

struct cyclist {
	int speed;
	int position;
	int lane;
	int lap;
};

// Seu codigo deve possuir um vetor compartilhado “pista” que tem um tamanho igual a d
int length = 250;
int n_cyclists = 1;
int final_lap = 160;
int lap = 0;
struct cyclist **pista;

bool has_blocking(struct cyclist *c) {
	// for (struct cyclist cy : pista[c->position + 1]) {

	// }
	return true;
}

void move(struct cyclist *c) {

}

void start(struct cyclist *c) {
	while(c->lap < 160) {
		move(c);
	}
}

void setCyclistPosition(struct cyclist *c, int position) {

}

void setStartPosition(struct cyclist *c) {
	
}

void *prepareAndStart(void *arg) {
	struct cyclist *c;
	c = (struct cyclist* ) arg;

	c = (struct cyclist* ) malloc(sizeof(struct cyclist));	
	c->speed = 30;
	c->lap = 0;

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

void prepareLap(struct cyclist c) {
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

int main(int argc, char **argv) {
    struct cyclist *cyclists[n_cyclists]; 
    srand(time(NULL)); 

    createThreads(n_cyclists, cyclists);

    return EXIT_SUCCESS;
}