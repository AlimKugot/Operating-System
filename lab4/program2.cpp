#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

using namespace std;


int main(int argc, char* argv[]) {
    cout << "Program 2. Старт" << endl;
    cout << "Program 2. Текущий ID: " << getpid() << endl;
    cout << "Program 2. Parent ID: " << getppid() << endl;
    int rv;
    pid_t pid;

    // error
    if ((pid = fork()) < 0) {
        handle_error_en(0, "fork error");
        // child
    } else if (pid == 0) {
        cout << "Program 2. Child ID: " << getpid() << endl;

        string animal_key = "animal";
        string animal_value =
                    "┊┊┊┊┊╭╭╭╮╮╮┊┊┊┊ \n"
                    "┊┊┊┊┊╰╰╲╱╯╯┊┊┊┊ \n"
                    "┊┏╮╭┓╭━━━━━━╮┊┊ \n"
                    "┊╰╮╭╯┃┈┈┈┈┈┈┃┊┊ \n"
                    "┊┊┃╰━╯┈┈╰╯┈┈┃┊┊ \n"
                    "┊┊┃┈┈┈┈┈┈┈╰━┫┊┊ \n"
                    "╲╱╲╱╲╱╲╱╲╱╲╱╲╱╲ \n";
        string teacher_key = "Учитель";
        string teacher_value = "Широков Владимир Владимирович";

        string student_key = "Студентка";
        string student_value = "Dina";

        string animal =  animal_key + "=" + animal_value;
        string teacher = teacher_key + "=" + teacher_value;
        string student = student_key + "=" + student_value;

        char* const animal_p = const_cast<char*>(animal.c_str());
        char* const teacher_p = const_cast<char*>(teacher.c_str());
        char* const student_p = const_cast<char*>(student.c_str());

        char* const env[] = {animal_p, teacher_p, student_p, nullptr};
        execvpe("./p1.out", argv, env);
        // parrent
    } else do {
            if ((pid = waitpid(pid, &rv, WNOHANG)) == -1) {
                handle_error_en(rv, "waiting error");
            } else if (pid == 0) {
                cout << "Program 2. <-- Проверка: дочерний процесс активен -->" << endl;
                sleep(2);
            } else {
                if (WIFEXITED(rv)) {
                    const int exit_status = WEXITSTATUS(rv);
                    cout << "Program 2. Child exited with status: " << exit_status << endl;
                    cout << "Program 2. Конец" << endl;
                }
            }
        } while (pid == 0);
    return 0;
}
