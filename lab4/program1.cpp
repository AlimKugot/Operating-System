#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {
	cout << string(50, '.') << endl;
	cout << "Program 1. Start" << endl;
	cout << "Program 1. Current ID: " << getpid() << endl; 
	cout << "Program 1. Parent ID: " << getppid() << endl;

	cout << "Program 1. Author = "  << getenv("author") << endl;
	for (int i = 0; i < argc; i++) {
		cout << "Program 1. args: " << argv[i] << endl;
		sleep(1);
	}
	cout << "Program 1. end" << endl;
	cout << string(50, '.') << endl;
	return 0;
}
