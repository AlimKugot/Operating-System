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

	void* p1;
	void* p2;

	pthread_join(thr1, &p1);
	pthread_join(thr2, &p2);

	int* status1 = (int*) p1;
	int* status2 = (int*) p2;

	printf("Result of function 1 is %d\n", *(status1));
	printf("Result of function 2 is %d\n", *(status2));
	delete isEnd;
	delete status1;
	delete status2;
	return 0;
}
