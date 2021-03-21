#include <PongAI/administrator.hpp>
#include <PongAI/game_invite.hpp>

#include <boost/log/trivial.hpp>
#include <boost/thread/thread.hpp>
#include <SimpleAmqpClient/Channel.h>

std::string Administrator::get_game_exchange_name() const
{
    return std::string(getenv("GAME_EXECHANGE_NAME"));
}

void Administrator::notify_start_game()
{
}

std::string Administrator::create_new_game_id() const
{

    return create_random_string(16, "game.");
}

void Administrator::create_new_game(int number_of_players, FieldSize field_size)
{
    AmqpClient::Channel::ptr_t channel = create_channel();
    auto game_exchange_name = this->get_game_exchange_name();

    GameState game_invite(number_of_players);

    auto consumer = prepare_listen_exchange_topic(channel, game_exchange_name, game_invite.game_id);
    
    game_invite.intend = GameStateIntend::Requesting;
    publish_game_invite(channel, &game_invite);
    listnen_for_participants(channel, consumer);

    if (game_invite.intend != GameStateIntend::Starting)
    {
        auto game_exchange_name = get_game_exchange_name();
        game_invite.intend = GameStateIntend::Stale;
        BOOST_LOG_TRIVIAL(info) << "Sending game_state stale " << GameState::to_json(game_invite);

        AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create(
            GameState::to_json(game_invite));
        
        channel->BasicPublish(game_exchange_name, game_invite.game_id, message);
    }

}

void Administrator::publish_game_invite(const AmqpClient::Channel::ptr_t &channel,
                                        const GameState *game_state) const
{
    auto game_exchange_name = get_game_exchange_name();
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create();

    message->Body(GameState::to_json(*game_state));
    BOOST_LOG_TRIVIAL(info) << "Game invite send: " << std::endl
                            << GameState::to_json(*game_state);
    channel->BasicPublish(game_exchange_name, game_state->game_id, message);
}

bool Administrator::should_acknowledge(GameState *current_state,
                                       const GameState &request) const
{

    bool should_accept(true);

    if (current_state->participents.size() + 1 != request.participents.size())
    {
        should_accept = false;
    }

    for (int i = 0; i < request.participents.size(); ++i)
    {
        if (i >= current_state->participents.size() && should_accept)
        {
            current_state->participents.push_back(request.participents[i]);
        }
        else
        {
            should_accept &= request.participents[i].first == current_state->participents[i].first;
            should_accept &= request.participents[i].second == current_state->participents[i].second;
        };
    };
    return should_accept;
}

void Administrator::respond_game_invite_acceptance(const channel_ptr &channel,
                                                   GameState *current_game_state,
                                                   GameState *request,
                                                   const bool should_accept) const
{
    auto game_exchange_name = get_game_exchange_name();
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create();
    if (should_accept)
    {
        current_game_state->intend = GameStateIntend::Acknowledge;
        message->Body(GameState::to_json(*current_game_state));
        channel->BasicPublish(game_exchange_name, current_game_state->game_id, message);

        if (current_game_state->number_of_players < current_game_state->participents.size())
        {
            current_game_state->intend = GameStateIntend::Requesting;
        }
        else
        {
            current_game_state->intend = GameStateIntend::Starting;
        }
        message->Body(GameState::to_json(*current_game_state));
        channel->BasicPublish(game_exchange_name, current_game_state->game_id, message);
    }
    else
    {
        current_game_state->intend = GameStateIntend::Reject;
        BOOST_LOG_TRIVIAL(info) << "Sending game_state rejection" << GameState::to_json(*current_game_state);
        message->Body(GameState::to_json(*current_game_state));
        channel->BasicPublish(game_exchange_name, current_game_state->game_id, message);
    }
}

void Administrator::listnen_for_participants(const channel_ptr &channel,
                                             const std::string &consumer) const
{
    auto envelope = AmqpClient::Envelope::ptr_t();

    bool has_found_message;
    bool game_is_fufilled(false);
    int timeout_number = 5;
    GameState accepted_game_state;
    do
    {
        has_found_message = channel->BasicConsumeMessage(consumer, envelope, 500);

        if (has_found_message)
        {
            GameState game_state = GameState::from_json(envelope->Message()->Body());
            if (game_state.intend == GameStateIntend::Accepting)
            {
                bool should_accept = should_acknowledge(&accepted_game_state, game_state);
                respond_game_invite_acceptance(channel,
                                               &accepted_game_state,
                                               &game_state,
                                               should_accept);
            }
        }
        else
        {
            --timeout_number;
        }
    } while (timeout_number >= 0 && accepted_game_state.intend != GameStateIntend::Acknowledge);
}