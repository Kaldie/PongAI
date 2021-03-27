#include <PongAI/pong/referee.hpp>
#include <boost/shared_ptr.hpp>

#include <PongAI/messaging/messages/game_invite.hpp>
#include <PongAI/pong/objects/field.hpp>
#include <PongAI/pong/objects/paddle.hpp>
namespace pong
{

    void Referee::prepare_for_game(
        const messaging::messages::GameInvite &game_invite)
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

        for (auto i = 0; i < 1; ++i)
        {
            objects::Ball ball;
            ball.set_owner(get_name());
            field.add_ball(ball);
        }
    };
} // namespace pong
