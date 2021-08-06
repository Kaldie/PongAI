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
        typedef std::map<std::string, messages::GameState>::iterator ActiveGameIterator;
    private:
        std::map<std::string, messages::GameState> active_games;

        virtual std::string entity_type() const override { return "Referee"; };

        virtual bool prepare_for_game(
            const ::messaging::messages::GameInvite &game_invite,
            std::string *message) override;

        virtual std::string respond_on_game_message(
            const std::string &game_message);

        void _merge_actions(const messages::GameState& suggestion, ActiveGameIterator& active_game);
        bool _should_update_state(const std::string& game_id);
        bool _all_actions_are_available(ActiveGameIterator& active_game);
        void _update_game_state(messages::GameState& game_state);
    };


       
};

#endif // __REFEREE_H__