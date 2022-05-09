#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

using namespace std;


int pipe_arr[2];
const int BUFFER_SIZE = 256;


void* proc1(void* isEnd) {
    char buf[BUFFER_SIZE];
    cout << "Начинается чтение" << endl;
    int j = 0;
    while (!(*((bool*) isEnd))) {
        int rv = read(pipe_arr[0], buf, sizeof(buf));
        if (rv == -1) {
	    handle_error_en(-1, "reading buffer");
	    sleep(1);
        } else {
            cout << "принято: ";
            while (j < BUFFER_SIZE && buf[j] != '\0') {
                cout << buf[j++];
            }
	    if (j >= BUFFER_SIZE) j = 0;
            cout << endl;
        }
        sleep(1);
    }
    close(pipe_arr[0]);
    pthread_exit(NULL);
}


void* proc2(void* isEnd) {
    char buf[BUFFER_SIZE];
    cout << "Начинается заполнение" << endl;

    int j = 0;
    while (!(*((bool*) isEnd))) {
        // if buffer is full
        if (buf[BUFFER_SIZE - 1] != '\0') {
            cout << string(20, '-') << endl;
            cout << "Очистка буфера" << endl;
            cout << string(20, '-') << endl;

            //clean up
	    for (char & c : buf) {
		    c = '\0';
	    }
	    buf[0] = '\n';
            j = 1;
        }
	// date function
        gid_t id = getgid();
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	char today_date[100];
	strftime (today_date, 100, "%Y-%m-%d %H:%M:%S\n", tm);
	string msg = today_date;

	cout << "передано: ";
	// fill buffer
        for (char c : msg) {
            buf[j++] = c;
	    cout << c;
        }

	// wrtie 
        int rv = write(pipe_arr[1], buf, sizeof buf);
        if (rv == -1) {
	    handle_error_en(rv, "cannot write to buffer");
	    sleep(1);
        }
        sleep(1);
    }
    close(pipe_arr[1]);
    pthread_exit(NULL);
}



int main() {
    setlocale(LC_ALL, "Russian");
    cout << "Старт" << endl;

    pthread_t threads[2];

    int rv = pipe(pipe_arr);
    if (rv != 0) {
    	handle_error_en(rv, "creating pipe");
    } else if (rv == 0) {
        cout << "Успешное создание pipe[2]" << endl;
        sleep(1);
    }


    bool* isEnd = new bool;
    *isEnd = false;

    pthread_create(&threads[0], NULL, proc1, isEnd);
    pthread_create(&threads[1], NULL, proc2, isEnd);

    getchar();
    *isEnd = true;

    void* status1;
    void* status2;

    pthread_join(threads[0], &status1);
    pthread_join(threads[1], &status2);

    cout << "Конец" << endl;
    close(pipe_arr[0]);
    close(pipe_arr[1]);
    delete isEnd;
    return 0;
}
