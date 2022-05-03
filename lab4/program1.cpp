#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {
	cout << string(20, '-') << endl;
	cout << "Program 1. Start" << endl;
	cout << "Program 1. Current ID: " << getpid() << endl; 
	cout << "Program 1. Parent ID: " << getppid() << endl;

	// печатаем переменную окружения
	cout << "Program 1. OWNER=" << getenv("OWNER") << endl;

	for (int i = 0; i < argc; i++) {
		cout << "Progrma 1. Args: \t\t\t" << argv[i] << endl;
		sleep(1);
	}
	cout << "Program 1. End" << endl;
	cout << string(20, '-') << endl;
	return 0;
}
