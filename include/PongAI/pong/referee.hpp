#ifndef __REFEREE_H__
#define __REFEREE_H__

#include <PongAI/messaging/participant.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

namespace messaging::messages
{
    class GameInvite;
    typedef boost::shared_ptr<GameInvite> GameInvite_ptr;
};

namespace pong
{
    namespace objects
    {
        class Field;
    }
    typedef boost::shared_ptr<objects::Field> Field_ptr;

    class Referee : public ::messaging::Participant
    {
    private:
        std::map<std::string, Field_ptr> active_games;

        virtual std::string entity_type() const override { return "Referee"; };

        virtual bool prepare_for_game(
            const ::messaging::messages::GameInvite &game_invite,
            std::string* message) override;
    };
};

#endif // __REFEREE_H__