#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <boost/shared_ptr.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

class Heartbeat;
typedef AmqpClient::Channel::ptr_t channel_ptr;

class Entity
{
private:
    std::string name;
    

    channel_ptr connect(const char *hostname,
                        const int port,
                        const char *username,
                        const char *password) const;

    std::string initialise_heartbeat(const channel_ptr& channel) const;
    boost::shared_ptr<Heartbeat> listen_heartbeat(const std::string &queue_name, const AmqpClient::Channel::ptr_t& channel) const;
    void listen_and_respond_heartbeat_requests(const std::string& queue_name, const channel_ptr&) const;

    
    std::string create_heartbeat_topic() const;
    std::string create_heartbeat_topic(const std::string& entity_type) const;
    

public:
    Entity(/* args */);

    channel_ptr create_channel() const;

    std::string declare_administration_exchange(const AmqpClient::Channel::ptr_t& ) const;
    std::string create_administrative_exchange(const channel_ptr&) const;
    
    void manage_heartbeats();
    void request_heartbeat(const std::string& entity_type) const;

    virtual std::string get_entity_type() const {return "Entity";};
};

#endif // __ENTITY_H__