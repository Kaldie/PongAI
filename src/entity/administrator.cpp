#include <PongAI/administrator.hpp>
#include <PongAI/game_invite.hpp>
#include <PongAI/util.hpp>

#include <SimpleAmqpClient/Channel.h>

#include <boost/log/trivial.hpp>
#include <boost/thread/thread.hpp>

void Administrator::create_new_game(int number_of_players, FieldSize field_size)
{
    AmqpClient::Channel::ptr_t channel = create_channel();
    auto game_exchange_name = get_game_exchange_name();

    GameState game_invite(number_of_players);
    auto consumer = prepare_listen_exchange_topic(channel, game_exchange_name, game_invite.get_game_id());

    game_invite.intend = GameStateIntend::Requesting;
    send_game_state(channel, game_invite);
    listnen_for_participants(channel, consumer, &game_invite);

    if (game_invite.intend != GameStateIntend::Starting)
    {
        game_invite.intend = GameStateIntend::Stale;
        send_game_state(channel, game_invite);
    }
}

void Administrator::send_game_state(const channel_ptr &channel,
                                    const GameState &game_state) const
{
    auto game_exchange_name = get_game_exchange_name();
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create(
        GameState::to_json(game_state));
    channel->BasicPublish(game_exchange_name, game_state.get_game_id(), message);
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
    if (should_accept)
    {
        current_game_state->intend = GameStateIntend::Acknowledge;
        // Send the player an Acknowledge
        send_game_state(channel, *current_game_state);

        if (current_game_state->number_of_players < current_game_state->participents.size())
        {
            current_game_state->intend = GameStateIntend::Requesting;
        }
        else
        {
            current_game_state->intend = GameStateIntend::Starting;
        }
        // Ask for new players or give the start signal
        send_game_state(channel, *current_game_state);
    }
    else
    {
        request->intend = GameStateIntend::Reject;
        BOOST_LOG_TRIVIAL(info) << "Sending rejection" << std::endl
                                << GameState::to_json(*request);
        // Request is invalid
        send_game_state(channel, *current_game_state);
    }
}

void Administrator::listnen_for_participants(const channel_ptr &channel,
                                             const std::string &consumer,
                                             GameState *accepted_game_state) const
{
    auto envelope = AmqpClient::Envelope::ptr_t();

    int timeout_number = 5;
    do
    {
        bool has_found_message = channel->BasicConsumeMessage(consumer, envelope, 500);

        if (has_found_message)
        {
            GameState game_state = GameState::from_json(envelope->Message()->Body());
            if (game_state.intend == GameStateIntend::Accepting)
            {
                bool should_accept = should_acknowledge(accepted_game_state, game_state);
                respond_game_invite_acceptance(channel,
                                               accepted_game_state,
                                               &game_state,
                                               should_accept);
            }
        }
        else
        {
            --timeout_number;
        }
    } while (timeout_number >= 0 &&
             accepted_game_state->intend != GameStateIntend::Acknowledge);
}