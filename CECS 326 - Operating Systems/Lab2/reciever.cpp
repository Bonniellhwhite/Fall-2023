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
        cerr << "Usage: ./receiver <message_queue_id>" << endl;
        return 1;
    }

    int msgQueueId = atoi(argv[1]);

    // Output identifying message
    cout << "Receiver, PID " << getpid() << ", begins execution" << endl;
    cout << "Receiver with PID " << getpid() << " received message queue id " << msgQueueId << " through command line argument" << endl;

    // Retrieve a message from the message queue
    Message message;
    msgrcv(msgQueueId, &message, sizeof(message.mtext), 1, 0);
    cout << "Receiver with PID " << getpid() << " retrieved the following message from message queue" << endl;
    cout << message.mtext << endl;

    // Send an acknowledgement message to the message queue
    message.mtype = 2;  // acknowledgement message type
    strcpy(message.mtext, "Receiver acknowledges receipt of message");
    msgsnd(msgQueueId, &message, sizeof(message.mtext), 0);

    // Terminate
    cout << "Receiver with PID " << getpid() << " terminates" << endl;

    return 0;
}
