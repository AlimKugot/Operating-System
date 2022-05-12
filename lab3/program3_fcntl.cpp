#include <iostream>
#include <unistd.h> 
#include <pthread.h> // pipe
#include <cstring> // strerror
#include <sys/utsname.h> // uts function
#include <fcntl.h> // O_NONBLOCK


using namespace std;


int pipe_arr[2];
const int BUFFER_SIZE = 256;


void* proc1(void* isEnd) {
    char buf[BUFFER_SIZE];
    printf("Read function is starting\n"); 
    int j = 0;

    while (!(*((bool*) isEnd))) {
        int rv = read(pipe_arr[0], buf, sizeof(buf));
        if (rv == -1) {
	    perror("redaing buffer error");
	    sleep(1);
        } else {
	    printf("accepted: ");
            while (j < BUFFER_SIZE && buf[j] != '\0') {
		printf("%c", buf[j++]);
            }
	    if (j >= BUFFER_SIZE) j = 0;
	    printf("\n");
        }
        sleep(1);
    }
    close(pipe_arr[0]);
    pthread_exit(NULL);
}


void* proc2(void* isEnd) {
    char buf[BUFFER_SIZE];
    printf("Write function is starting\n"); 
    int j = 0;

    while (!(*((bool*) isEnd))) {
        // if buffer is full
        if (buf[BUFFER_SIZE - 1] != '\0') {
	    printf("\n\n\nClean up buffer\n\n\n");
	    memset(buf, 0, sizeof(buf));
            j = 0;
        }

	// uts function
	struct utsname uts;
	if (uname(&uts) < 0) {
		perror("uname() error");
		sleep(1);
	} 
	string msg = string(uts.sysname) + ", " + string(uts.nodename) + ", " + string(uts.machine) + "\n";

	printf("passed: ");
        for (char c : msg) {
            buf[j++] = c;
	    printf("%c", c);
        }

	// wrtie 
        int rv = write(pipe_arr[1], buf, sizeof buf);
        if (rv == -1) {
	    perror("writing buffer error");
	    sleep(1);
        }
        sleep(1);
    }
    close(pipe_arr[1]);
    pthread_exit(NULL);
}



int main() {
    printf("@Author Ekaterina Alexeeva\n");
    printf("Start\n");
    pthread_t threads[2];

    int rv = pipe(pipe_arr);
    if (rv != 0) {
	// Добавила strerror - замечание с прошлой сдачи работы
	strerror(rv);
    } else if (rv == 0) {
	printf("Success creating pipe[2]\n");
        sleep(1);
    }
    fcntl(pipe_arr[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_arr[1], F_SETFL, O_NONBLOCK);

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

    printf("End\n");
    close(pipe_arr[0]);
    close(pipe_arr[1]);
    delete isEnd;
    return 0;
}
