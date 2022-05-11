#include <stdio.h> // printf 
#include <unistd.h> // sleep 
#include <pthread.h> // mutex

pthread_mutex_t mutex;


void* proc1(void* isEnd) {
	printf("Thread 1: start\n");
	while (!(*((bool*) isEnd))) {
		while (pthread_mutex_trylock(&mutex) != 0) {
			perror("Trylock 1");
			sleep(1);
		}
		printf("Locked 1\n");

		for (int i = 0; i < 5; i++) {
			printf("%d\n", 1);
			sleep(1);
		}

		pthread_mutex_unlock(&mutex);
		printf("Unlocked 1\n");
		sleep(1);
	}
	pthread_exit(NULL);
}


void* proc2(void* isEnd) {
	printf("Thread 2: start\n");
	while (!(*((bool*) isEnd))) {
		while (pthread_mutex_trylock(&mutex) != 0) {
			perror("Trylock 2");
			sleep(1);
		}
		printf("Locked 2\n");

		for (int i = 0; i < 5; i++) {
			printf("%d\n", 2);
			sleep(1);
		}

		pthread_mutex_unlock(&mutex);
		printf("Unlocked 2\n");
		sleep(1);
	}
	pthread_exit(NULL);
}


int main() {
	printf("Start.\n");
	pthread_t p1, p2;
	pthread_mutex_init(&mutex, NULL);

	bool* isEnd = new bool;
	*isEnd = false;

	pthread_create(&p1, NULL, &proc1, (void*) isEnd);
	pthread_create(&p2, NULL, &proc2, (void*) isEnd);

	getchar();
	*isEnd = true;

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	pthread_mutex_destroy(&mutex);
	printf("END.\n");
	delete isEnd;
	return 0;
}
