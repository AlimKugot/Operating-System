#include <iostream>
#include <unistd.h>

using namespace std;




int main(int argc, char* argv[]) {
	cout << string(20, '-') << endl;
	cout << "Program 1. Start" << endl;
	cout << "Program 1. Current ID: " << getpid() << endl; 
	cout << "Program 1. Parent ID: " << getppid() << endl;
	for (int i = 1; i <= 5; i++) {
		cout << "Program 1." << "\t\t\t" << i << endl;
		sleep(1);
	}
	cout << "Program 1. End" << endl;
	cout << string(20, '-') << endl;
	return 25;
}
