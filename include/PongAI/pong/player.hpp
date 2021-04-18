#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <PongAI/messaging/participant.hpp>
#include <string>

namespace messaging {
    namespace messages
    {
        class GameInvite;
    }
}


namespace pong
{
    class Player : public messaging::Participant
    {
    private:
        
        virtual std::string entity_type() const override { return "Player"; };

        virtual bool prepare_for_game(
            const messaging::messages::GameInvite &game_invite,
            std::string* message) override {return false;};

        virtual std::string respond_on_game_message(
            const std::string &game_message);

    public:
        virtual void play_game(){};
    };
};

#endif // __PLAYER_H__