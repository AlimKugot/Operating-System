#include <iostream>
#include <thread>
#include <pthread.h>

using namespace std;

pthread_key_t key1;
pthread_key_t key2;


void* proc1(void* isEnd) {
    cout << "Start proc1" << endl;
    int* PROC_1_EXIT_SUCCESS = new int;
    *PROC_1_EXIT_SUCCESS = 1;
    pthread_key_create(&key1, nullptr);
    pthread_setspecific(key1, PROC_1_EXIT_SUCCESS);

    int count = 0;

    while (!(*((bool*) isEnd))) {
        count++;
        cout << "Proc1 with count: " << count << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    cout << "End proc1 with count: " << count << endl;
    int* status = ((int*) pthread_getspecific(key1));
    pthread_key_delete(key1);
    pthread_exit(status);
}


void* proc2(void* isEnd) {
    cout << "Start proc2" << endl;
    int* PROC_2_EXIT_SUCCESS = new int;
    *PROC_2_EXIT_SUCCESS = 2;
    pthread_key_create(&key2, nullptr);
    pthread_setspecific(key2, &PROC_2_EXIT_SUCCESS);

    int count = 0;

    while (!(*((bool*) isEnd))) {
        count++;
        cout << "Proc2 with count: " << count << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    cout << "End proc2 with count: " << count << endl;
    int* status = ((int*) pthread_getspecific(key2));
    pthread_key_delete(key2);
    pthread_exit(status);
}


int main() {
    cout << "Starting c++ program" << endl;

    bool* isEnd = new bool;
    *isEnd = false;

    const int NUM_THREADS = 2;
    pthread_t threads[NUM_THREADS];

    cout << "created thread 1" << endl;
    if (pthread_create(&threads[0], NULL, proc1, isEnd) == -1) {
        cout << "Cannot create thread 1" << endl;
        return -1;
    }

    cout << "created thread 2" << endl;
    if (pthread_create(&threads[1], NULL, proc2, isEnd) == -1) {
        cout << "Cannot create thread 2" << endl;
        return -1;
    }

    cout << "Enter something to kill all threads: " << endl;
    getchar();

    *isEnd = true;

    int* status1;
    int* status2;

    int ret1 = pthread_join(threads[0], reinterpret_cast<void **>(&status1));
    int ret2 = pthread_join(threads[1], reinterpret_cast<void **>(&status2));

    cout << string(20, '-') << endl;

    cout << "Proc 1 end working with ret " << ret1 << " end status " << *((int*) status1) << endl;
    cout << "Proc 2 end working with ret " << ret2 << " end status " << *((int*) status2) << endl;

    delete isEnd;
    return 0;
}
