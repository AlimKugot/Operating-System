#include <iostream>
#include <pthread.h>

using namespace std;


void* proc1(void* isEnd) {
	cout << "Start proc1" << endl;
	int count = 0;

	while (!(*((bool*) isEnd))) {
		count++;
	}

	cout << "End proc1 with count: " << count << endl;
	pthread_exit(NULL);
}


void* proc2(void* isEnd) {
	cout << "Start proc2" << endl;
	int count = 0;

	while (!(*((bool*) isEnd))) {
		count++;
	}	

	cout << "End proc2 with count: " << count << endl;
	pthread_exit(NULL);
}


int main() {
	cout << "Starting c++ program" << endl;

	bool* isEnd = new bool;
	*isEnd = false;

	const int NUM_THREADS = 2;
	pthread_t threads[NUM_THREADS];

	cout << "created thread 1" << endl;
	if (pthread_create(&threads[0], NULL, proc1, isEnd) == -1) {
	       	cout << "Cannot create thread 1" << endl;
		return -1;
	}

	cout << "created thread 2" << endl;
	if (pthread_create(&threads[1], NULL, proc2, isEnd) == -1) {
		cout << "Cannot create thread 2" << endl;
		return -1;
	}

	cout << "Enter something to kill all threads: " << endl;
	getchar();

	*isEnd = true;

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	return 0;
}
