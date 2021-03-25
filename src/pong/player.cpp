#include <PongAI/pong/player.hpp>
#include <PongAI/pong/messages/game_state.hpp>

namespace pong
{

    std::string Player::respond_on_game_message(
        const std::string &game_message)
    {
        messages::GameState::from_json(game_message);
        return "";
    }

} // namespace pong