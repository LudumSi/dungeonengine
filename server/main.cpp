#include <thread>
#include <mutex>
#include <queue>
#include <iostream>

std::mutex outbox_mutex;
std::mutex inbox_mutex;

std::queue<std::string> outbox;
std::queue<std::string> inbox;

void process_inbox() {
    std::string message;
    do {
        message = "";
        
        inbox_mutex.lock();
        
        if(!inbox.empty()) {
            message = inbox.front();
            inbox.pop();
            std::cout << message << std::endl;
        }
        
        inbox_mutex.unlock();
        
    } while(message != "logout");
}

void process_outbox() {
    std::string message;
    do {
        std::cin >> message;
        
        outbox_mutex.lock();
        outbox.push(message);
        outbox_mutex.unlock();

    } while(message != "logout");

}

void chat_listener() {
    return;
}

int main() {

    std::cout << "Starting Server" << std::endl;

    // Open two threads
    // Thread 1 establishes client connections and listens/sends messages
    // Thread 2 will "process" messages (i.e. add messages to the outgoing queue and remove messages from the incoming queue and print)
    std::thread network_manager(chat_listener);
    std::thread inboxer(process_inbox);
    std::thread outboxer(process_outbox);

    network_manager.join();
    inboxer.join();
    outboxer.join();

    std::cout << "Complete" << std::endl;
    return 0;
}