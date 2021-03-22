#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <PongAI/player.hpp>

int main(int, char**) {
    std::cout << "Hello, world!" << std::endl << "This is Player!\n";
    
    boost::shared_ptr<Participant> player = boost::make_shared<Participant>();

    // AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");
    // std::string consumer_tag = channel->BasicConsume("my_queue", "");
    // AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer_tag);
}
