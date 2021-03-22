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
    } // namespace messages
    
}

namespace pong
{
    class Player : messaging::Participant
    {
    private:
        
        Ball ball;
        Paddle paddle;
        Field field;
        
        
        virtual std::string entity_type() const override { return "Player"; };
        std::string prepare_listen_game(const messaging::messages::GameInvite& game_invite);
        
        void respond_on_input();


    public:
        virtual void play_game();
    };
};

#endif // __PLAYER_H__