#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {
	cout << string(50, '.') << endl;
	cout << "Program 1. Старт" << endl;
	cout << "Program 1. Текущий ID: " << getpid() << endl; 
	cout << "Program 1. Parent ID: " << getppid() << endl;

	const char* teacher = "Учитель";
	const char* me = "Студентка";
	const char* animal = "animal";
	cout << "Program 1. " << teacher << " = "  << getenv(teacher) << endl;
	cout << "Program 1. " << me << " = " <<  getenv(me) << endl;
	cout << "Program 1. " << animal << ":" << endl << getenv(animal) << endl;

	for (int i = 0; i < argc; i++) {
		cout << "Progrma 1. Аргументы: " << argv[i] << endl;
		sleep(1);
	}
	cout << "Program 1. Конец" << endl;
	cout << string(50, '.') << endl;
	return 0;
}
