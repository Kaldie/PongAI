#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

int main(int, char**) {
    std::cout << "Hello, world!\n";

    std::string consumer_tag = channel->BasicConsume("my_queue", "");
    Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer_tag);
    // Alternatively:
    Envelope::ptr_t envelope;
    channel->BasicConsumeMessage(consumer_tag, envelope, 10); // 10 ms timeout
    // To ack:
    channel->BasicAck(envelope);
    // To cancel:
    channel->BasicCancel(consumer_tag);


}
