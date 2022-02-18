#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

using namespace std;

pthread_mutex_t mutex;


void* func1(void* isEnd) {
	cout << "Starting thread 1" << endl;
	while (!(*((bool*) isEnd))) {
		pthread_mutex_lock(&mutex);
		cout << "locked mutex 1" << endl;

		for (int i = 0; i < 5; i++) {
			cout << 1 << endl;
			sleep(1);
		}

		pthread_mutex_unlock(&mutex);
		cout << "unlocked mutex 1" << endl;
		sleep(1);
	}
	pthread_exit(NULL);
}


void* func2(void* isEnd) {
	cout << "Starting thread 2" << endl;
	struct timespec wait_time;
	while (!(*((bool*) isEnd))) {
		pthread_mutex_lock(&mutex);
		cout << "locked mutex 2" << endl;

		for (int i = 0; i < 5; i++) {
			cout << 2 << endl;
			sleep(1);
		}

		pthread_mutex_unlock(&mutex);
		cout << "unlocked mutex 2" << endl;
		sleep(1);
	}
	pthread_exit(NULL);
}


int main() {
	cout << "Starting program" << endl;

	pthread_t p1, p2;
	pthread_mutex_init(&mutex, NULL);

	bool* isEnd1 = new bool;
	bool* isEnd2 = new bool;
	*isEnd1 = false;
	*isEnd2 = false;

	pthread_create(&p1, NULL, &func1, (void*) isEnd1);
	pthread_create(&p2, NULL, &func2, (void*) isEnd1);

	getchar();
	*isEnd1 = true;
	*isEnd2 = true;

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	pthread_mutex_destroy(&mutex);
	delete isEnd1;
	delete isEnd2;
	return 0;
}
