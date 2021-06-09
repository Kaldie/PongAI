#ifndef __REFEREE_H__
#define __REFEREE_H__

#include <PongAI/messaging/participant.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

namespace messaging::messages
{
    class GameInvite;

};

namespace pong
{
    namespace objects
    {
        class Field;

    }

    namespace messages
    {
        class GameState;
    }

    typedef boost::shared_ptr<objects::Field> Field_ptr;

    class Referee : public ::messaging::Participant
    {
    private:
        std::map<std::string, messages::GameState> active_games;

        virtual std::string entity_type() const override { return "Referee"; };

        virtual bool prepare_for_game(
            const ::messaging::messages::GameInvite &game_invite,
            std::string *message) override;

        virtual std::string respond_on_game_message(
            const std::string &game_message);

        void merge_actions(const messages::GameState& suggestion);
        bool should_update_state(const std::string& game_id);
        void update_game_state(std::string& game_id);
    };   
};

#endif // __REFEREE_H__