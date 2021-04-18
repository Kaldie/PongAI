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
        std::string* message)
    {
        auto field = boost::make_shared<pong::objects::Field>();
        field->set_owner(get_name());

        for (auto participent : game_invite.participents)
        {
            if (participent.first == EntityType::PlayerType)
            {
                objects::Paddle paddle;
                paddle.set_owner(participent.second);
                field->add_paddle(paddle);
            }
        }

        // TODO: #4 in the invite we need to set the field specifics
        for (auto i = 0; i < 1; ++i)
        {
            objects::Ball ball;
            ball.set_owner(get_name());
            field->add_ball(ball);
        }

        active_games.insert(std::make_pair(game_invite.get_game_id(), field));
        (*message) = messages::GameState::to_json(messages::GameState(field));
        return true;
    };
} // namespace pong
