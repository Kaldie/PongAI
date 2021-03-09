#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SimpleAmqpClient/SimpleAmqpClient.h>


class Entity {
private:

    const static std::string entity_type;
    /* data */
    AmqpClient::Channel::ptr_t channel;
    void connect(const char* hostname, const int port, const char* username, const char* password);
    void create_exchange(const std::string&, const std::string&) const;
    std::string create_queue(const std::string&) const;
    
public:
    Entity(/* args */);

    void connect();
    AmqpClient::Channel::ptr_t get_channel() const {return channel;};
    void initialise_heartbeat();

};




#endif // __ENTITY_H__