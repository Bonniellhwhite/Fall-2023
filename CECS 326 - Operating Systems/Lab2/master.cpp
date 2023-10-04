#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./master <number_of_processes>" << endl;
        return 1;
    }

    int numProcesses = atoi(argv[1]);

    // Output identifying message
    cout << "Master, PID " << getpid() << ", begins execution" << endl;

    // Acquire a message queue
    key_t key = ftok("master.cpp", 'A');
    int msgQueueId = msgget(key, IPC_CREAT | 0666);

    // Fork sender and receiver processes
    for (int i = 0; i < numProcesses; ++i) {
        if (fork() == 0) {  // Child process
            execl("./sender", "./sender", to_string(msgQueueId).c_str(), NULL);
            execl("./receiver", "./receiver", to_string(msgQueueId).c_str(), NULL);
            exit(0);
        }
    }

    // Output process information
    cout << "Master acquired a message queue, id " << msgQueueId << endl;
    cout << "Master created " << numProcesses << " child processes to serve as sender and receiver" << endl;

    // Wait for all child processes to terminate
    for (int i = 0; i < numProcesses * 2; ++i) {
        wait(NULL);
    }

    // Remove the message queue
    msgctl(msgQueueId, IPC_RMID, NULL);

    // Output termination message
    cout << "Master received termination signals from all child processes, removed message queue, and terminates" << endl;

    return 0;
}
