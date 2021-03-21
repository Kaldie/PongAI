#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

class Heartbeat;

namespace AmqpClient
{

    class Channel;

}
typedef boost::shared_ptr<AmqpClient::Channel> channel_ptr;

class Entity
{
private:
    std::string id;

    channel_ptr connect(const char *hostname,
                        const int port,
                        const char *username,
                        const char *password) const;

    std::string create_heartbeat_queue(const channel_ptr &channel, const std::string &heartbeat_topic) const;

    void listen_and_respond_heartbeat_requests(const std::string &queue_name, const channel_ptr &) const;

    std::string create_heartbeat_topic_name() const;
    std::string create_heartbeat_topic_name(const std::string &entity_type) const;

    void request_heartbeat(const std::string &entity_type, const channel_ptr &channel) const;

    virtual std::string entity_type() const { return "Entity"; };
public:
    Entity(/* args */);
    std::string get_name() const {return entity_type() + "_" + id;};

    channel_ptr create_channel() const;

    std::string prepare_listen_exchange_topic(
        const channel_ptr &channel,
        const std::string &exchange_name,
        const std::string &topic_name) const;

    std::string prepare_listen_admin_exchange_topic(
        const channel_ptr &channel,
        const std::string &topic_name) const;

    std::string prepare_listen_heartbeat_topic(
        const channel_ptr &channel, 
        const std::string &entity_type) const;

    std::string prepare_listen_heartbeat_topic(
        const channel_ptr &channel) const;

    void manage_heartbeats() const;

    std::vector<std::string> find_entities(const std::string& entity_type) const;

    std::string get_entity_type() const {return entity_type();};

};

#endif // __ENTITY_H__