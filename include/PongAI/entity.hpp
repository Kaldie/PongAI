#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <boost/shared_ptr.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

class Heartbeat;

namespace AmqpClient {
    
    namespace Channel {
        shared_ptr<
    }
}
typedef AmqpClient::Channel::ptr_t channel_ptr;

class Entity
{
private:
    std::string name;

    channel_ptr connect(const char *hostname,
                        const int port,
                        const char *username,
                        const char *password) const;

    std::string create_heartbeat_queue(const channel_ptr &channel, const std::string& heartbeat_topic) const;
    boost::shared_ptr<Heartbeat> listen_heartbeat_request(const std::string &queue_name, const channel_ptr &channel) const;

    void listen_and_respond_heartbeat_requests(const std::string &queue_name, const channel_ptr &) const;

    std::string create_heartbeat_topic_name() const;
    std::string create_heartbeat_topic_name(const std::string &entity_type) const;

    void request_heartbeat(const std::string &entity_type, const channel_ptr &channel) const;

public:
    Entity(/* args */);

    channel_ptr create_channel() const;

    std::string declare_administration_exchange(const channel_ptr &) const;

    void manage_heartbeats();
    std::vector<std::string> find_entities(const std::string &entity_type) const;

    virtual std::string get_entity_type() const { return "Entity"; };
};

#endif // __ENTITY_H__