#ifndef __REFEREE_H__
#define __REFEREE_H__

#include <PongAI/messaging/participant.hpp>

namespace messaging::messages
{
    class GameInvite;
    typedef boost::shared_ptr<GameInvite> GameInvite_ptr;
};

namespace pong
{
    class Referee : public ::messaging::Participant
    {
    private:
        virtual std::string entity_type() const override { return "Referee"; };
         virtual void prepare_for_game(
             const ::messaging::messages::GameInvite &game_invite) override;
    };
};
#endif // __REFEREE_H__