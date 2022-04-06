#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;



int main(int argc, char* argv[]) {
	cout << "Program 2. Start" << endl;
	cout << "Program 2. Curent ID: " << getpid() << endl;
	cout << "Program 2. Parent ID: " << getppid() << endl;
	int status;
	pid_t pid;


	if ((pid = fork()) < 0) {
		perror("fork error");
	} else if (pid == 0) {
		// do it once
		cout << "Program 2. Child ID: " << getpid() << endl;

		// We need to compile program2 before starting program1
		// execlp("g++", "g++", "program1.cpp", "-o", "p1.out",  (char *) NULL); 

		execlp("./p1.out", "./p1.out", (char *) NULL);
		// exit(0);  // there is exit here
	} else do {
		// do it repeatly
		if ((pid = waitpid(pid, &status, WNOHANG)) == -1) {
			perror("waiting error");
		} else if (pid == 0) {
			cout << "Program 2. ----- child is still running -----" << endl;
			sleep(2);
		} else {
			if (WIFEXITED(status)) {
				cout << "Program 2. Child exited with status: " << status << endl;
				cout << "Program 2. Ending program 2." << endl;
			}
		}
	} while (pid == 0);
	return 0;
}
