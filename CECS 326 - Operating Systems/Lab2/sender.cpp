#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

using namespace std;

struct Message {
    long mtype;
    char mtext[1024];
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./sender <message_queue_id>" << endl;
        return 1;
    }

    int msgQueueId = atoi(argv[1]);

    // Output identifying message
    cout << "Sender, PID " << getpid() << ", begins execution" << endl;
    cout << "Sender with PID " << getpid() << " received message queue id " << msgQueueId << " through command line argument" << endl;

    // Prompt user for input
    cout << "Sender with PID " << getpid() << ": Please input your message" << endl;
    string userMessage;
    getline(cin, userMessage);

    // Send the message to the message queue
    Message message;
    message.mtype = 1;  // message type
    strcpy(message.mtext, userMessage.c_str());
    msgsnd(msgQueueId, &message, sizeof(message.mtext), 0);

    // Wait for an acknowledgement from the receiver
    msgrcv(msgQueueId, &message, sizeof(message.mtext), 2, 0);
    cout << "Sender with PID " << getpid() << " receives the following acknowledgement message" << endl;
    cout << message.mtext << endl;

    // Terminate
    cout << "Sender with PID " << getpid() << " terminates" << endl;

    return 0;
}
