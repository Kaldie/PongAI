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

        // Add players
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
        ActiveGameIterator active_game = active_games.find(state.get_field().get_id());
        if (active_game != active_games.end())
        {
            _merge_actions(state, active_game);
            if (_all_actions_are_available(active_game))
            {
                _update_game_state(active_game->second);
            }
        }
        return "here";
    };

    void Referee::_merge_actions(const messages::GameState &suggestion, ActiveGameIterator &active_game)
    {
        auto has_found_new_action = false;

        for (auto action : suggestion.get_actions())
        {
            has_found_new_action |= active_game->second.add_action(action.second);
        }
    }

    bool Referee::_all_actions_are_available(ActiveGameIterator &active_game)
    {
        bool all_actions_available = true;
        Actions actions = active_game->second.get_actions();
        for (auto player : active_game->second.get_players())
        {
            if (actions.find(player) == actions.end())
            {
                all_actions_available = false;
            }
        }

        return all_actions_available;
    }

} // namespace pong
