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

    void listnen_for_participants(const channel_ptr &channel,
                                  const std::string &consumer,
                                  GameState *accepted_game_state) const;

    bool should_acknowledge(GameState *current_state,
                            const GameState &request) const;

    void respond_game_invite_acceptance(const channel_ptr &channel,
                                        GameState *current_game_state,
                                        GameState *request,
                                        const bool should_accept) const;

    void send_game_state(const channel_ptr &channel,
                         const GameState &game_state) const;

    virtual std::string entity_type()
        const override { return "Administrator"; };

public:
    void create_new_game(int number_of_players,
                         FieldSize field_size);


};

#endif // __ADMINISTRATOR_H__