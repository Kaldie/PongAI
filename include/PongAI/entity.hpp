#ifndef __ENTITEE_H__
#define __ENTITEE_H__

#include <SimpleAmqpClient/SimpleAmqpClient.h>


class Entity {
private:
    /* data */
    AmqpClient::Channel::ptr_t channel;
    bool connected = false;

    void connect(const char* hostname);
    
public:
    Entity(/* args */);

    void connect();

};



#endif // __ENTITEE_H__