#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;


int main(int argc, char* argv[]) {
    cout << "Program 2. Start" << endl;
    cout << "Program 2. Current ID: " << getpid() << endl;
    cout << "Program 2. Parent ID: " << getppid() << endl;
    pid_t pid;

    // error
    if ((pid = fork()) < 0) {
	perror("fork error");
	sleep(1);
    // child
    } else if (pid == 0) {
        cout << "Program 2. Child ID: " << getpid() << endl;
        char* const env[] = {"author=Ekaterina", nullptr};
        execvpe("./p1.out", argv, env);
    // parrent
    } else do {
	    int rv;
            if ((pid = waitpid(pid, &rv, WNOHANG)) == -1) {
		perror("waiting error");
            } else if (pid == 0) {
                cout << "Program 2. <-- Check: child proccess is alive -->" << endl;
                sleep(2);
            } else {
                if (WIFEXITED(rv)) {
                    const int exit_status = WEXITSTATUS(rv);
                    cout << "Program 2. Child exited with status: " << exit_status << endl;
                    cout << "Program 2. End" << endl;
                }
            }
    } while (pid == 0);
    return 0;
}
