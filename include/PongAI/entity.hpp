#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SimpleAmqpClient/SimpleAmqpClient.h>

typedef boost::shared_ptr<AmqpClient::Channel> channel_ptr;

class Entity
{
private:
    std::string name;
    

    channel_ptr connect(const char *hostname,
                        const int port,
                        const char *username,
                        const char *password) const;

    void create_exchange(const std::string &, const std::string &) const;
    std::string create_queue(const std::string &) const;

    std::string initialise_heartbeat();

    
    std::string create_queue_name() const;
    std::string create_queue_name(const std::string& entity_type) const;
    std::string create_administrative_exchange() const;

public:
    Entity(/* args */);

    channel_ptr create_channel() const;
    void manage_heartbeats();

    void listen_and_respond_heartbeat(const std::string&) const;
    void request_heartbeat(const std::string& entity_type) const;

    virtual std::string get_entity_type() const {return Entity::entity_type;};

    const static std::string entity_type;
};

#endif // __ENTITY_H__