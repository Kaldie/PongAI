#include <PongAI/pong/referee.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <PongAI/messaging/messages/game_invite.hpp>
#include <PongAI/pong/messages/game_state.hpp>
#include <PongAI/pong/objects/ball.hpp>
#include <PongAI/pong/objects/field.hpp>
#include <PongAI/pong/objects/goal.hpp>
#include <PongAI/pong/objects/paddle.hpp>

namespace pong
{

    bool Referee::prepare_for_game(
        const messaging::messages::GameInvite &game_invite,
        std::string *message)
    {
        pong::objects::Field field;
        field.set_owner(get_name());

        for (auto participent : game_invite.participents)
        {
            if (participent.first == EntityType::PlayerType)
            {
                objects::Paddle paddle;
                paddle.set_owner(participent.second);
                field.add_paddle(paddle);
            }
        }

        // TODO: #4 in the invite we need to set the field specifics
        for (auto i = 0; i < 1; ++i)
        {
            objects::Ball ball;
            ball.set_owner(get_name());
            field.add_ball(ball);
        }

        auto game_state = messages::GameState(field);
        game_state.set_game_id(game_invite.get_game_id());
        active_games.insert(std::make_pair(game_invite.get_game_id(), game_state));
        (*message) = messages::GameState::to_json(game_state);
        return true;
    };

    std::string Referee::respond_on_game_message(
        const std::string &game_message)
    {
        messages::GameState state = messages::GameState::from_json(game_message);
        auto corrisponding_active_game = active_games.find(state.get_field().get_id());
        if (corrisponding_active_game != active_games.end())
        {
            merge_actions(state);
            if (all_actions_are_available(state.))
            {
                update_known_state(*corrisponding_active_game)
            }
        }
    };

    void Referee::merge_actions(const messages::GameState &suggestion)
    {
        auto state_iterator = active_games.find(suggestion.get_game_id());
        if (active_games.find(suggestion.get_game_id()) == active_games.end())
        {
            return;
        }

        auto game_state = state_iterator->second;
        for (auto seggested_action : suggestion.get_actions())
        {
            for (auto known_action : game_state.get_actions())
            {
                if (seggested_action.first == known_action.first) {

                }
            }
        }

        state_iterator->second = game_state;
    }

} // namespace pong
