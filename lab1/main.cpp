#include <iostream>
#include <pthread.h>

using namespace std;

bool isEnd1 = false;
bool isEnd2 = false;

void* proc1(void* pointer) {
	cout << "Start proc1" << endl;
	int count = 0;
	while (!isEnd1) {
		count++;
	}
	cout << "End proc1 with count: " << count << endl;
	pthread_exit(NULL);
}

void* proc2(void* pointer) {
	cout << "Start proc2" << endl;
	int count = 0;
	while (!isEnd2) {
		count++;
	}	
	cout << "End proc2 with count: " << count << endl;
	pthread_exit(NULL);
}

int main() {
	cout << "Starting c++ program" << endl;

	const int NUM_THREADS = 2;
	pthread_t threads[NUM_THREADS];

	cout << "created thread 1" << endl;
	if (pthread_create(&threads[0], NULL, proc1, NULL) == -1) {
	       	cout << "Cannot create thread 1" << endl;
		return -1;
	}

	cout << "created thread 2" << endl;
	if (pthread_create(&threads[1], NULL, proc2, NULL) == -1) {
		cout << "Cannot create thread 2" << endl;
		return -1;
	}

	cout << "Enter something to kill all threads: " << endl;
	getchar();

	isEnd1 = true;
	isEnd2 = true;

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	return 0;
}
