#include <stdio.h> /* поддержка функции printf */
#include <unistd.h> /* поддержка функции sleep */
#include <pthread.h> /* поддержка многопоточности */


void* function_1(void* isEnd) {
	printf("Starting function 1\n");
	for (int i = 0; !(*((bool*) isEnd)); i++) {
		printf("function 1 is running: %d\n", i);
		sleep(1);
	}
	int* success = new int;
	*success = 0;
	pthread_exit((void*) success);
}


void* function_2(void* isEnd) {
	printf("Starting function 2\n");
	for (int i = 0; !(*((bool*) isEnd)); i++) {
		printf("function 2 is running: %d\n", i);
		sleep(1);
	}	
	int* success = new int;
	*success = 0;
	pthread_exit((void*) success);
}


int main() {
	printf("@Author Ekaterina Alexeeva\n");
	pthread_t thr1;
	pthread_t thr2;
	bool* isEnd = new bool;
	*isEnd = false;


	printf("Creating thread 1\n");
	int ret = pthread_create(&thr1, NULL, &function_1, isEnd);
	if (ret != 0) perror("create first");

	printf("Creating thread 2\n");
	ret = pthread_create(&thr2, NULL, &function_2, isEnd);
	if (ret != 0) perror("create second");

	getchar();

	*isEnd = true;

	void* status1;
	void* status2;

	pthread_join(thr1, &status1);
	pthread_join(thr2, &status2);

	printf("Result of function 1 is %d\n", *((int*) status1));
	printf("Result of function 2 is %d\n", *((int*) status2));
	delete isEnd;
	return 0;
}
