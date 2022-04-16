#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;


int pipe_arr[2];
const int BUFFER_SIZE = 256;


void* proc1(void* isEnd) {
	char buf[BUFFER_SIZE];
	cout << "Starting reading proc" << endl;
	while (!(*((bool*) isEnd))) {
		int rv = read(pipe_arr[0], buf, sizeof(buf));
		if (rv == -1) {
			cerr << "Error reading buffer" << endl;
		} else {
			cout << "Success reading buffer: ";
			for (int i = 0; i < BUFFER_SIZE && buf[i] != '\0'; i++) {
				cout << buf[i];
			}
			cout << endl;
		}	
		sleep(1);
	}
	close(pipe_arr[0]);
	pthread_exit(NULL);
}


void* proc2(void* isEnd) {
	char buf[BUFFER_SIZE];
	cout << "Starting writing proc" << endl;


	const char* FILE_NAME = "output.txt"; 
	FILE *fp = fopen(FILE_NAME, "rb+");
	if (fp != NULL) {
		cout << "Deleting file: " << FILE_NAME << endl;
		remove(FILE_NAME);
	}
	cout << "Creating file: " << FILE_NAME << endl;
	fp = fopen(FILE_NAME, "wb");


	while (!(*((bool*) isEnd))) {
		int id = (int) getgid();
		string msg = "The group ID of the calling process is " + to_string(id) + "\n";

		// fill local buffer
		for (int i = 0; i < msg.size(); i++) {
			buf[i] = msg[i]; 
		}

		// write into buffer
		int rv = write(pipe_arr[1], buf, sizeof buf);
		if (rv == -1) {
			cerr << "Cannot write into into buffer" << endl;
		} else {
			cout << "Writing to output.txt" << endl;
			fwrite(buf, sizeof (char), sizeof (buf), fp);
		}
		sleep(1);
	}
	close(pipe_arr[1]);
	pthread_exit(NULL);
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
		cout << "Success creating pipe array" << endl;
		sleep(1);
	} else {
		perror("Error: strange pipe rv " + rv);
	}


	pthread_create(&threads[0], NULL, proc1, isEnd); 
	pthread_create(&threads[1], NULL, proc2, isEnd); 

	getchar();
	*isEnd = true;

	void* status1;
	void* status2; 

	pthread_join(threads[0], &status1);
	pthread_join(threads[1], &status2);

	cout << "End of program" << endl;
	delete isEnd;
	return 0;
}
