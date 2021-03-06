#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

sem_t semaphore;


int PROC_1_EXIT_SUCCESS = 1;
int PROC_2_EXIT_SUCCESS = 2;


void* proc1(void* isEnd) {
	while (!(*((bool*) isEnd))) {
		sem_wait(&semaphore); 
		cout << "locked semaphore 1" << endl;
		for (int i = 0; i < 5; i++) {
			cout << 1 << endl;
			sleep(1);
		}
		cout << "unlocked semaphore 1" << endl;
		sem_post(&semaphore);
		sleep(1);
	}
	pthread_exit(&PROC_1_EXIT_SUCCESS);
}


void* proc2(void* isEnd) {

	while (!(*((bool*) isEnd))) {
		sem_wait(&semaphore); 
		cout << "locked semaphore 2" << endl;
		for (int i = 0; i < 5; i++) {
			cout << 2 << endl;
			sleep(1);
		}
		cout << "unlocked semaphore 2" << endl;
		sem_post(&semaphore);
		sleep(1);
	}
	pthread_exit(&PROC_2_EXIT_SUCCESS);
}

int main() {
	cout << "Starting c++ program" << endl;
	
	// from https://docs.oracle.com/cd/E19455-01/806-5257/sync-42602/index.html
	// 'When pshared is nonzero, the semaphore can be shared by other processes.'
	//	
	// third arg = count of threads for this semaphore
	sem_init(&semaphore, 0, 1);

	bool* isEnd = new bool;
	*isEnd = false;

	const int NUM_THREADS = 2;
	pthread_t threads[NUM_THREADS];


	if (pthread_create(&threads[0], NULL, proc1, isEnd) != -1) {
		cout << "created thread 1" << endl;
	} else {
	       	cerr << "Cannot create thread 1" << endl;
		return -1;
	}
	

	if (pthread_create(&threads[1], NULL, proc2, isEnd) != -1) {
		cout << "created thread 2" << endl;
	} else {
		cerr << "Cannot create thread 2" << endl;
		return -1;
	}


	getchar();
	*isEnd = true;

	void* status1;
	void* status2; 

	//int ret1 = pthread_join(threads[0], &status1);
	int ret2 = pthread_join(threads[1], &status2);
	int ret1 = pthread_join(threads[0], &status1);

	sem_destroy(&semaphore);

	cout << endl << string(20, '-') << endl; 
	cout << "Proc 1 end working with ret " << ret1 << " end status " << *((int*) status1) << " (expected " << PROC_1_EXIT_SUCCESS << ")" << endl;  
	cout << "Proc 2 end working with ret " << ret2 << " end status " << *((int*) status2) << " (expected " << PROC_2_EXIT_SUCCESS << ")" << endl;  
	return 0;
}
