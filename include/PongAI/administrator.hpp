#ifndef __ADMINISTRATOR_H__
#define __ADMINISTRATOR_H__

#include <PongAI/default_include.hpp>
#include <PongAI/entity.hpp>
#include <vector>
#include <string>

namespace AmqpClient
{

    class Channel;

}

typedef boost::shared_ptr<AmqpClient::Channel> channel_ptr;

class Administrator : public Entity
{
private:
    void notify_new_game();
    void notify_start_game();
    void notify_end_game();
    void create_game_exchange_and_queue(const std::string &) const;
    void publish_game_invite(const channel_ptr &channel,
                             const std::string &) const;
                             
    std::string create_new_game_id() const;
    std::string get_game_exchange_name() const;
    std::vector<std::string> listnen_for_participants(const channel_ptr &channel,
                                                      const std::string &consumer) const;

public:
    void create_new_game(int number_of_players, FieldSize field_size);
    virtual std::string get_entity_type() const override { return "Administrator"; };
};

#endif // __ADMINISTRATOR_H__