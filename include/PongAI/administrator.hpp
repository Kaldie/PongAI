#ifndef __ADMINISTRATOR_H__
#define __ADMINISTRATOR_H__

#include <PongAI/default_include.hpp>
#include <PongAI/entity.hpp>
#include <string>

namespace AmqpClient
{

    class Channel;

};

typedef boost::shared_ptr<AmqpClient::Channel> channel_ptr;

class GameState;

class Administrator : public Entity
{
private:
    void notify_new_game();
    void notify_start_game();
    void notify_end_game();
    void publish_game_invite(const channel_ptr &channel,
                             const GameState *game_state) const;

    std::string create_new_game_id() const;
    std::string get_game_exchange_name() const;
    void listnen_for_participants(const channel_ptr &channel,
                             const std::string &consumer) const;

    bool should_acknowledge(GameState *current_state,
                            const GameState &request) const;

    void respond_game_invite_acceptance(const channel_ptr &channel,
                                        GameState *current_game_state,
                                        GameState *request,
                                        const bool should_accept) const;

public:
    void create_new_game(int number_of_players, FieldSize field_size);
    virtual std::string get_entity_type() const override { return "Administrator"; };
};

#endif // __ADMINISTRATOR_H__