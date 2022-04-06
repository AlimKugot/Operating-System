#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

using namespace std;


int pipe_arr[2];


int PROC_1_EXIT_SUCCESS = 1;
int PROC_2_EXIT_SUCCESS = 2;


void* proc1(void* isEnd) {
	char buf[256];
	cout << "Starting reading proc" << endl;
	while (!(*((bool*) isEnd))) {
		int rv = read(pipe_arr[0], buf, sizeof(buf));
		if (rv == -1) {
			cerr << "Error reading buffer" << endl;
		} else {
			for (int i = 0; i < 256 && buf[i] != '\0'; i++) {
				cout << buf[i];
			}
		}	
		sleep(1);
	}
	close(pipe_arr[0]);
	pthread_exit(&PROC_1_EXIT_SUCCESS);
}


void* proc2(void* isEnd) {
	char buf[256];

	// create file (delete last version if exists)
	const char* FILE_NAME = "output.txt"; 
	FILE *fp = fopen(FILE_NAME, "rb+");
	if (fp != NULL) {
		cout << "Deleting file: " << FILE_NAME << endl;
		remove(FILE_NAME);
	}
	cout << "Creating file - " << FILE_NAME << endl;
	fp = fopen(FILE_NAME, "wb");


	int j = 0;
	while (!(*((bool*) isEnd))) {
		// get info
		struct hostent *lh = gethostbyname("www.github.com");
		string msg_string = string(lh->h_name) + string(" ") + string(lh->h_addr);
		const char* msg = msg_string.c_str();

		// fill local buffer
		for (int i = 0; i < 256; i++) {
			buf[i] = *(msg + i); 
		}

		// write into buffer
		int rv = write(pipe_arr[1], buf, sizeof buf);
		if (rv == -1) {
			cerr << "Error with writing into buffer" << endl;
		} else {
			// log into file 
			fwrite(buf, sizeof (char), sizeof (buf), fp);
		}
		sleep(1);
	}
	close(pipe_arr[1]);
	pthread_exit(&PROC_2_EXIT_SUCCESS);
}



int main() {
	cout << "Starting c++ program" << endl;

	bool* isEnd = new bool;
	*isEnd = false;

	const int NUM_THREADS = 2;
	pthread_t threads[NUM_THREADS];


	int rv = pipe(pipe_arr);
	if (rv < 0) {
		cout << "Error: creating pipe" << endl;
		sleep(1);
		exit(rv);
	} else if (rv == 0) {
		cout << "End of file" << endl;
		sleep(1);
	} else {
		cout << "Success creating pipe array" << endl;
	}


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

	int ret1 = pthread_join(threads[0], &status1);
	int ret2 = pthread_join(threads[1], &status2);

	cout << endl << string(20, '-') << endl; 
	cout << "Proc 1 end working with ret " << ret1 << " end status " << *((int*) status1) << " (expected " << PROC_1_EXIT_SUCCESS << ")" << endl;  
	cout << "Proc 2 end working with ret " << ret2 << " end status " << *((int*) status2) << " (expected " << PROC_2_EXIT_SUCCESS << ")" << endl;  

	delete isEnd;
	return 0;
}
