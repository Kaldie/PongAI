#include <PongAI/game_invite.hpp>
#include <PongAI/player.hpp>
#include <PongAI/util.hpp>
#include <SimpleAmqpClient/Channel.h>
#include <boost/log/trivial.hpp>

void Player::find_and_participate()
{

    channel_ptr channel = create_channel();
    auto consumer = prepare_listen_exchange_topic(channel,
                                                  get_game_exchange_name(),
                                                  "game.#");
    do
    {
        BOOST_LOG_TRIVIAL(info) << "Listening for new game";
        listen_and_accept_game_invite(channel, consumer);
    } while (!has_acknowledged_invite(channel, consumer));
}

bool Player::has_acknowledged_invite(const channel_ptr &channel,
                                     const std::string &consumer) const
{
    bool has_accepted(false);
    AmqpClient::Envelope::ptr_t envelope;
    bool has_seen_message = channel->BasicConsumeMessage(consumer, envelope, 500);
    if (has_seen_message)
    {
        GameState game_state = GameState::from_json(envelope->Message()->Body());

        if ((game_state.intend == GameStateIntend::Acknowledge ||
             game_state.intend == GameStateIntend::Reject) &&
            game_state.participents.back().first == EntityType::PlayerType &&
            game_state.participents.back().second == get_name())
        {
            has_accepted = game_state.intend == GameStateIntend::Acknowledge;
            BOOST_LOG_TRIVIAL(info) << "Has been accepted: " << has_accepted;
        }
        else
        {
            has_accepted = has_acknowledged_invite(channel, consumer);
        }
    }
    return has_accepted;
}

void Player::listen_and_accept_game_invite(const channel_ptr &channel,
                                           const std::string &consumer) const
{
    AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer);
    GameState game_state = GameState::from_json(envelope->Message()->Body());

    if (game_state.intend == GameStateIntend::Requesting)
    {
        BOOST_LOG_TRIVIAL(info) << "Heared a game invite!";

        game_state.participents.push_back(std::make_pair(
            EntityType::PlayerType, get_name()));

        game_state.intend = GameStateIntend::Accepting;

        BOOST_LOG_TRIVIAL(info) << "Sending an invite accept message";

        channel->BasicPublish(
            get_game_exchange_name(),
            game_state.get_game_id(),
            AmqpClient::BasicMessage::Create(GameState::to_json(game_state)));
    }
}