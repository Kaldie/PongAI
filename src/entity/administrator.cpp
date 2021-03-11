#include <PongAI/administrator.hpp>
#include <PongAI/game_invite.hpp>

#include <boost/log/trivial.hpp>
#include <boost/thread/thread.hpp>

std::string Administrator::create_administration_exchange() const
{
    AmqpClient::Channel::ptr_t channel = create_channel();
    const char *ADMINISTRATION_EXECHANGE_NAME = getenv("ADMINISTRATION_EXECHANGE_NAME");
    if (channel)
    {
        channel->DeclareExchange(ADMINISTRATION_EXECHANGE_NAME,
                                 AmqpClient::Channel::EXCHANGE_TYPE_FANOUT,
                                 false,
                                 true);
    }
    return ADMINISTRATION_EXECHANGE_NAME;
}

void Administrator::create_game_exchange_and_queue(const std::string &game_id) const
{
    AmqpClient::Channel::ptr_t channel = create_channel();
    if (channel)
    {
        // Create an auto-deletion exchange of type topic
        channel->DeclareExchange(
            game_id,
            AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, false, false, true);
    }
}

void Administrator::notify_start_game()
{
}

std::string Administrator::create_new_game_id() const
{

    return create_random_string(16,"game_");
}

void Administrator::create_new_game(int number_of_players, FieldSize field_size)
{
    auto referee_exchange = create_administration_exchange();
    auto game_id = create_new_game_id();
    create_game_exchange_and_queue(game_id);
    publish_game_invite(game_id);
}

void Administrator::publish_game_invite(const std::string &game_id) const
{
    AmqpClient::Channel::ptr_t channel = create_channel();
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create();
    GameInvite game_invite(game_id, 2);
    message->Body(GameInvite::to_json(game_invite));
    BOOST_LOG_TRIVIAL(info) << "Game invite send: " << std::endl
                            << GameInvite::to_json(game_invite);
    channel->BasicPublish("referee", "", message);
}


const std::string Administrator::entity_type = "Administrator";


