#include <PongAI/administrator.hpp>
#include <PongAI/game_invite.hpp>

#include <boost/log/trivial.hpp>
#include <boost/thread/thread.hpp>
#include <SimpleAmqpClient/Channel.h>

std::string Administrator::get_game_exchange_name() const
{
    return getenv("GAME_EXECHANGE_NAME");
}

void Administrator::create_game_exchange_and_queue(const std::string &game_id) const
{
    AmqpClient::Channel::ptr_t channel = create_channel();
    // Create an auto-deletion exchange of type topic
    channel->DeclareExchange(
        game_id,
        AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, false, false, true);
}

void Administrator::notify_start_game()
{
}

std::string Administrator::create_new_game_id() const
{

    return create_random_string(16, "game_");
}

void Administrator::create_new_game(int number_of_players, FieldSize field_size)
{
    AmqpClient::Channel::ptr_t channel = create_channel();
    auto game_exchange_name = this->get_game_exchange_name();
    auto game_id = create_new_game_id();
    auto consumer = prepare_listen_exchange_topic(channel, game_exchange_name, game_id);
    publish_game_invite(channel, game_id);
    auto listnen_for_participants(channel);
}

void Administrator::publish_game_invite(const AmqpClient::Channel::ptr_t &channel, const std::string &game_id) const
{
    auto game_exchange_name = get_game_exchange_name();
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create();
    GameInvite game_invite(game_id, 2);
    message->Body(GameInvite::to_json(game_invite));
    BOOST_LOG_TRIVIAL(info) << "Game invite send: " << std::endl
                            << GameInvite::to_json(game_invite);
    channel->BasicPublish(game_exchange_name, game_id, message);
}

std::vector<std::string> Administrator::listnen_for_participants(const channel_ptr &channel,
                                                                 const std::string &consumer) const
{
    auto envelope = AmqpClient::Envelope::ptr_t();
    bool has_found_message;
    do
    {
        has_found_message = channel->BasicConsumeMessage(consumer, envelope, 500);

    } while (has_found_message)
}