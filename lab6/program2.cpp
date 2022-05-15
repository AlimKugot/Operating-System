#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <semaphore.h>
#include <sys/mman.h>
#include <signal.h>		// system errors
#include <netdb.h>		// gethostbyname
#include <sys/stat.h>		// For mode constants
#include <fcntl.h>           	// For O_* constants 




#define err_exit(msg)    do { perror(msg); exit(EXIT_FAILURE); } while (0)


using namespace std;


const int BUF_SIZE = 1024;			// buffer to write/read
int fd;						// descriptor
const string shmp_path = "/shared_memory";	// path to shared memory
string sem_write_name = "sem_write"; 		// name for named sem
string sem_read_name = "sem_read"; 		// name for named sem
sem_t* sem_read;				// sem to read from shared memory
sem_t* sem_write;				// sem to write to shared memory
char buf[BUF_SIZE]; 				// to hold data
char* data;



/*
 * @func: clean up semaphores and shared memory
 */
void clean_up() {
	cout << string(30, '-') << endl;
	cout << "\t Clean up \t" << endl;
	cout << endl;

	int rv;

	if ((rv = sem_unlink(sem_read_name.c_str())) != 0) {
		perror("sem_read unlink");
	} else {
		cout << "Success clean:\t unlink sem_read" << endl;
	}


	if ((rv = sem_unlink(sem_write_name.c_str())) != 0) {
		perror("sem_write unlink");
	} else {
		cout << "Success clean:\t unlink sem_write" << endl;
	}


	if ((rv = sem_destroy(sem_read)) != 0) {
		perror("sem_read destroy");
	} else {
		cout << "Success clean:\t destroy sem_read" << endl;
	}

	if ((rv = sem_destroy(sem_write)) != 0) {
		perror("sem_write destroy");	
	} else {
		cout << "Success clean:\t destroy sem_write" << endl;
	}

	if ((rv = shm_unlink(shmp_path.c_str())) != 0) {
		perror("shm_unlink");
	} else {
		cout << "Success clean:\t shm_unlink" << endl;
	}


	if ((rv = munmap(data, BUF_SIZE)) != 0) {
		perror("munmap");
	} else {
		cout << "Success clean:\t munmap" << endl;
	}

	if ((rv = close(fd)) != 0) {
		perror("close fd");		
	} else {
		cout << "Success clean:\t close fd" << endl;		
	}

	cout << string(30, '-') << endl;
}


/*
 * @func: my sig handler to clean up allocated memory
 */
void sig_handler(int signo) {
	clean_up();
	exit(0);
}



/*
 * @func: to create thread
 */
void* proc_read(void* isEnd) {
	cout << "proc_read:\t is starting" << endl;
	int i = 0;
	while (!(*((bool*) isEnd))) {	
		cout << "proc_read:\t waiting sem_write" << endl;
		if (sem_wait(sem_write) == -1) {
			perror("proc2 sem_wait(sem_write)");
			sleep(1);
		} else {
			cout << "proc_read:\t is reading" << endl;
		}

		memcpy(buf, data, strlen(data));

		cout << "Proc2 accepted msg:\t ";
		for (int i = 0; i < BUF_SIZE && buf[i] != '\0'; i++) {
			cout << buf[i];
		}
		cout << endl;

		if (sem_post(sem_read) == -1) {
			perror("proc 2 sem_post(sem_read) error");
			sleep(1);
		} else {
			cout << "Proc2:\t sem_read posted" << endl;
		}

		i++;
		sleep(1);
	}
	cout << "Proc write exit" << endl;
	pthread_exit(0);
}






int main() {
	cout << "Starting program" << endl;
	bool* isEnd = new bool;
	*isEnd = false;

	// set default error handler to my own
	//
	// (e.g. call this function when pressed Ctrl + C)
	signal(SIGINT, sig_handler);


	// create shared memory
	if ((fd = shm_open(shmp_path.c_str(), O_CREAT | O_RDWR, 0644)) == -1) {
		err_exit("shm_open");
	}
	cout << "Success shmp_open" << endl;

	// allocate memory for shared_memory
	if (ftruncate(fd, BUF_SIZE) == -1) {
		err_exit("ftruncate");
	}
	cout << "Success ftruncate" << endl;

	// maps our struct shmp to allocated data	
	data = (char*) mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (data == MAP_FAILED) {
       		err_exit("mmap");	
	}
	cout << "Success mmap" << endl;

	// sem open
	sem_read = sem_open(sem_read_name.c_str(), O_CREAT, 0644); 
	sem_write = sem_open(sem_write_name.c_str(), O_CREAT, 0644); 

	// create thread
	pthread_t th1;
	int rv;
	if ((rv = pthread_create(&th1, NULL, proc_read, isEnd)) != 0) {
		cout << strerror(rv) << endl;
		exit(rv);
	}
	cout << "Success pthread_t create" << endl;


	getchar();
	*isEnd = true;

	pthread_join(th1, NULL);
	clean_up();
	delete isEnd;
	return 0;
}
