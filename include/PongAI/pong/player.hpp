#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <PongAI/messaging/participant.hpp>
#include <PongAI/pong/objects/ball.hpp>
#include <PongAI/pong/objects/paddle.hpp>
#include <PongAI/pong/objects/field.hpp>
#include <string>

namespace messaging {
    namespace messages
    {
        class GameInvite;
    }
}


namespace pong
{
    typedef boost::shared_ptr<messaging::messages::GameInvite> GameInvite_ptr;

    class Player : messaging::Participant
    {
    private:
        
        // objects::Ball ball;
        // objects::Paddle paddle;
        objects::Field field;
        
        
        virtual std::string entity_type() const override { return "Player"; };

        virtual void prepare_for_game(
            const GameInvite_ptr &game_invite) const override {};

        virtual std::string respond_on_game_message(
            const std::string &game_message);


    public:
        virtual void play_game(){};
    };
};

#endif // __PLAYER_H__