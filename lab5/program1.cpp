#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

int kbhit();

int main() {
	cout << "Starting program" << endl;
	sem_unlink("output.txt");

	sem_t* sem = sem_open("output.txt", O_CREAT, 0644, 1);
	if (sem == SEM_FAILED) {
		perror("sem open p1 failed");
	}

	ofstream fout;
	fout.open("output.txt", ofstream::app);

	// uncomment it if program force quited and sem wasn't close
	//sem_post(sem);

	cout << "Starting while p1" << endl;
	while (kbhit() == 0) {
		if (sem_wait(sem) == 0) {
			// locked
			cout << "sem by program 1 is locked" << endl;
		} else {
			perror("sem wait p1");
		}

		for (int i = 0; i < 5; i++) {
			cout << 1 << endl;
			fout << '1' << flush;
			sleep(1);
		}
		sem_post(sem);
		cout << "sem by program 1 is unlocked" << endl;
		sleep(1);
	}
	fout.close();
	sem_close(sem);
	sem_unlink("output.txt");
	return 0;
}


/*
 * Just copied the code. I hope it works :D
*/
int kbhit() {
	struct timeval tv; fd_set
	read_fd; /* Do not wait at all, not even a microsecond */
	tv.tv_sec=0;
	tv.tv_usec=0; /* Must be done first to initialize read_fd */
	FD_ZERO(&read_fd); /* Makes select() ask if input is ready: *
	0 is the file descriptor for stdin */
	FD_SET(0,&read_fd); /* The first parameter is the number of the *
	largest file descriptor to check + 1. */
	if(select(1, &read_fd, NULL, /*No writes*/ NULL, /*No exceptions*/
	&tv) == -1)
	return 0; /* An error occured */

	/* read_fd now holds a bit map of files that are *
	readable. We test the entry for the standard *
	input (file 0). */
	if(FD_ISSET(0,&read_fd)) /* Character pending on stdin */
	return 1; /* no characters were pending */
	return 0;
}
