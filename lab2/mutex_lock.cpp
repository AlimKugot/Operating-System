#include <iostream>
#include <pthread.h>

using namespace std;

pthread_mutex_t mutex;
int N = 0;

void* proc(void* arg) {
	pthread_mutex_lock(&mutex);
	N++;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}



int main() {
	const int NUM_THREADS = 5;
	pthread_t thread_pool[NUM_THREADS];

	pthread_mutex_init(&mutex, NULL);
	
	for (int i = 0; i < NUM_THREADS; i++) {
		cout << "Creating thread " << i << endl;
		int rv = pthread_create(&thread_pool[i], NULL, proc, NULL);
		if (rv == -1) {
			cerr << "Cannot create thread " << i << endl;
			return -1;
		}
	}
	cout << "Enter something to END program: ";
	getchar();

	for (int i = 0; i < NUM_THREADS; i++) {
		cout << "Killing thread " << i << endl;
		pthread_join(thread_pool[i], NULL);
	}
	
	cout << endl << endl << "Result is " << N << endl;
	return 0;
}
