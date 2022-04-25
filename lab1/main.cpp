#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;


void* proc1(void* isEnd) {
	cout << "Старт потока 1" << endl;
	int i = 0;
	string msg = "";

	while (!(*((bool*) isEnd))) {
		i++;
		msg += to_string(i) + "+";	
		cout << "Функция 1: " << msg << endl;
		sleep(1);
	}
	pthread_exit(0);
}


void* proc2(void* isEnd) {
	cout << "Старт потока 2" << endl;
	int i = 0;
	string msg = "";

	while (!(*((bool*) isEnd))) {
		i++;
		msg += to_string(i) + "_";	
		cout << "Функция 2: " << msg << endl;
		sleep(1);
	}	
	pthread_exit(0);
}


int main() {
	setlocale(LC_ALL, "Russian");
	cout << "Старт программы" << endl;

	bool* isEnd = new bool;
	*isEnd = false;

	pthread_t threads[2];

	cout << "Создание потока 1" << endl;
	if (pthread_create(&threads[0], NULL, proc1, isEnd) == -1) {
		perror("невозоможно создать поток 1");
	}

	cout << "Создание потока 2" << endl;
	if (pthread_create(&threads[1], NULL, proc2, isEnd) == -1) {
		perror("невозоможно создать поток 2");
	}

	getchar();

	*isEnd = true;

	void* status1;
	void* status2;

	pthread_join(threads[0], &status1);
	pthread_join(threads[1], &status2);

	cout << "Статус выполнения первой функции  " << ((int*) status1) << endl;
	cout << "Статус выполнения второй функции  " << ((int*) status2) << endl;
	cout << "Конец программы" << endl;
	return 0;
}
