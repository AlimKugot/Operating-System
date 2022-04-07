#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;



string convert_argv_to_string(int argc, char* argv[]) {
	string argv_line = "";
	int i;
	for (i = 0; i < argc - 1; i++) {
		argv_line += string(argv[i]) + " ";
	}
	argv_line += argv[i];
	return argv_line;
}


int main(int argc, char* argv[]) {
	cout << "Program 2. Start" << endl;
	cout << "Program 2. Curent ID: " << getpid() << endl;
	cout << "Program 2. Parent ID: " << getppid() << endl;
	int status;
	pid_t pid;
	string argv_line = convert_argv_to_string(argc, argv);
	cout << argv_line << endl;

	if ((pid = fork()) < 0) {
		perror("fork error");
	} else if (pid == 0) {
		cout << "Program 2. Child ID: " << getpid() << endl;
		//string terminal_comand = "g++ program1.cpp -o p1.out && . && ./p1.out " + argv_line;
		//cout << terminal_comand << endl;
		//execlp("sh", "sh", "-c", terminal_comand, NULL);
		execlp("sh", "sh", "-c", "g++ program1.cpp -o p1.out && . && ./p1.out", NULL);

		/*
		// We need to compile program2 before starting program1
		if ((child_pid = fork()) == 0) {
			cout << "Program 2. Compiling program 1." << endl;
			execlp("g++", "g++", "program1.cpp", "-o", "p1.out",  (char *) NULL); 
		} else if (child_pid > 0) {
			cout << "Program 2. Executing program 1." << endl;
			execlp("sh", "sh", "-c", "g++ program1.cpp -o p1.out && p1.out", NULL);
			//execlp("./p1.out", "./p1.out", argc, argv_line, (char *) NULL);
		} else {
			perror("child fork error");
		}
		*/
	} else do {
		// do it repeatly
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
