#include <iostream>
#include <time.h>
#include <thread>
#include <pthread.h>

using namespace std;

pthread_mutex_t mutex;
int N = 0;

void* proc(void* arg) {
	struct timespec wait_time;
	wait_time.tv_sec = ((long long) time(0)) + 3l;
	int rv = pthread_mutex_timedlock(&mutex, &wait_time);

	if (((long) arg) == 3) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}

	if (rv != 0) {
		cerr << "We've problems with mutex_timedlock. Thread " << (int*) arg << endl;
	}
	N++;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}



int main() {
	const int NUM_THREADS = 5;
	pthread_t thread_pool[NUM_THREADS];

	pthread_mutex_init(&mutex, NULL);
	
	for (long i = 0; i < NUM_THREADS; i++) {
		cout << "Creating thread " << i << endl;
		int rv = pthread_create(&thread_pool[i], NULL, proc, (void *) i);
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
