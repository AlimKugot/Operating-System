#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <signal.h>		// system errors
#include <netdb.h>		// gethostbyname
#include <sys/stat.h>		// For mode constants
#include <fcntl.h>           	// For O_* constants 




#define err_exit(msg)    do { perror(msg); exit(EXIT_FAILURE); } while (0)


using namespace std;


const int BUF_SIZE = 1024;

struct shm_buf {
	int fd = 0;				// descriptor
	string sem_read_name = "/sem_read"; 	// name for named sem
	string sem_write_name = "/sem_write"; 	// name for named sem
	sem_t* sem_read;			// sem to read from shared memory
	sem_t* sem_write;			// sem to write to shared memory		 
	size_t cnt; 				// Number of bytes used in buf	
	char buf[BUF_SIZE]; 			// to hold data
}; 


const string shmp_path = "/shared_memory";
struct shm_buf* shmp;




/*
 * @func: clean up semaphores and shared memory
 */
void clean_up() {
	sem_unlink((*shmp).sem_read_name.c_str());
	sem_unlink((*shmp).sem_write_name.c_str());
	sem_destroy((*shmp).sem_read);
	sem_destroy((*shmp).sem_write);

	shm_unlink(shmp_path.c_str());
	munmap(shmp, sizeof(*shmp));
	close((*shmp).fd);
}

/*
 * @func: my sig handler to clean up allocated memory
 */
void sig_handler(int signo) {
	clean_up();
	exit(0);
}

/*
 * @func: create msg
 */
const char* create_msg() {
	struct hostent *lh = gethostbyname("www.github.com");
	string msg_string = string(lh->h_name) + string(" ") + string(lh->h_addr);
	const char* msg = msg_string.c_str();
	return msg;
}


/*
 * @func: to create thread
 */
void* proc1(void* isEnd) {
	while (!(*((bool*) isEnd))) {	
		const char* msg = create_msg();
		cout << "Result: " << *msg << endl;

		const int N = sizeof(*msg) / sizeof(char);
		for (int i = 0; i < BUF_SIZE && i < N; i++) {
			(*shmp).buf[i] = *(msg + i);
		}
		sleep(1);
	}
	pthread_exit(0);
}






int main() {
	cout << "Starting program" << endl;
	bool* isEnd = new bool;
	*isEnd = false;
	int rv;

	pthread_t th1;

	// set default error handler to my own
	//
	// (e.g. call this function when pressed Ctrl + C)
	signal(SIGINT, sig_handler);


	// create shared memory
	if (((*shmp).fd = shm_open(shmp_path.c_str(), O_CREAT | O_RDWR, 0644)) == -1) {
		err_exit("shm_opne");
	}

	// allocate memory for shared_memory
	if ((rv = ftruncate((*shmp).fd, sizeof(shm_buf))) == -1) {
		err_exit("ftruncate");
	}

	// maps our struct shmp to allocated data	
	mmap(shmp, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, (*shmp).fd, 0);
	
	if (shmp == MAP_FAILED) {
       		err_exit("mmap");	
	}

	// sem open
	(*shmp).sem_read = sem_open((*shmp).sem_read_name.c_str(), O_CREAT, S_IRUSR | S_IWUSR); 
	(*shmp).sem_write = sem_open((*shmp).sem_write_name.c_str(), O_CREAT, S_IRUSR | S_IWUSR); 

	if (pthread_create(&th1, NULL, proc1, isEnd) != -1) {
		err_exit("pthread_create");	
	}


	getchar();
	*isEnd = true;

	void* status;
	pthread_join(th1, &status);

	cout << "Proc 1 end working with end status " << *((int*) status) << endl;
	clean_up();
	delete isEnd;
	return 0;
}
