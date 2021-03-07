#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

int main(int, char**) {
    std::cout << "Hello, world!" << std::endl << "This is Referee!\n";

    // AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");
    // std::string consumer_tag = channel->BasicConsume("my_queue", "");
    // AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer_tag);
}
