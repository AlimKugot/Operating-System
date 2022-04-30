#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <error.h>

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)


using namespace std;

pthread_mutex_t mutex;


void* proc1(void* isEnd) {
	cout << "Поток 1 начал работу" << endl;
	while (!(*((bool*) isEnd))) {
		while (pthread_mutex_trylock(&mutex) != 0) {
			cout << "Trylock не заблокировал 1" << endl;
			sleep(1);
		}
		cout << "Заблокирован 1" << endl;

		for (int i = 0; i < 5; i++) {
			cout << 1 << endl;
			sleep(1);
		}

		pthread_mutex_unlock(&mutex);
		cout << "Разаблокирован 1" << endl;
		sleep(1);
	}
	pthread_exit(NULL);
}


void* proc2(void* isEnd) {
	cout << "Поток 2 начал работу" << endl;
	while (!(*((bool*) isEnd))) {
		while (pthread_mutex_trylock(&mutex) != 0) {
			cout << "Trylock не заблокировал 2" << endl;
			sleep(1);
		}
		cout << "Заблокирован 2" << endl;

		for (int i = 0; i < 5; i++) {
			cout << 2 << endl;
			sleep(1);
		}

		pthread_mutex_unlock(&mutex);
		cout << "Разаблокирован 2" << endl;
		sleep(1);
	}
	pthread_exit(NULL);
}


int main() {
	setlocale(LC_ALL, "Russian");
	cout << "Старт" << endl;

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
	cout << "Конец" << endl;
	delete isEnd;
	return 0;
}
