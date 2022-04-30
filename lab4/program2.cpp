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
		cout << "Program 2. Child ID: " << getpid() << endl;
		execve("./p1.out", argv, NULL);
	} else do {
		if ((pid = waitpid(pid, &status, WNOHANG)) == -1) {
			perror("waiting error");
		} else if (pid == 0) {
			cout << "Program 2. ----- child is still running -----" << endl;
			sleep(2);
		} else {
			if (WIFEXITED(status)) {
				const int exit_status = WEXITSTATUS(status);
				cout << "Program 2. Child exited with status: " << exit_status << endl;
				cout << "Program 2. End" << endl;
			}
		}
	} while (pid == 0);
	return 0;
}
