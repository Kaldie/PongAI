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

    GameInvite game_invite(number_of_players);
    auto consumer = prepare_listen_exchange_topic(channel, game_exchange_name, game_invite.get_game_id());

    game_invite.intend = GameInviteIntend::Requesting;
    send_game_state(channel, game_invite);
    listnen_for_participants(channel, consumer, &game_invite);

    if (game_invite.intend != GameInviteIntend::Starting)
    {
        game_invite.intend = GameInviteIntend::Stale;
        send_game_state(channel, game_invite);
    }
}

void Administrator::send_game_state(const channel_ptr &channel,
                                    const GameInvite &game_state) const
{
    auto game_exchange_name = get_game_exchange_name();
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create(
        GameInvite::to_json(game_state));
    channel->BasicPublish(game_exchange_name, game_state.get_game_id(), message);
}

bool Administrator::should_acknowledge(GameInvite *current_state,
                                       const GameInvite &request) const
{

    bool should_accept(true);
    BOOST_LOG_TRIVIAL(debug) << "should acknowledge";
    if (current_state->participents.size() + 1 != request.participents.size())
    {
        BOOST_LOG_TRIVIAL(info) << "Rejecting, participent size was wrong!";
        should_accept = false;
    }

    for (int i = 0; i < request.participents.size(); ++i)
    {
        auto new_participent = request.participents[i];
        if (i >= current_state->participents.size() && should_accept)
        {
            BOOST_LOG_TRIVIAL(debug) << "before requires_entity";
            if (current_state->requires_entity(new_participent.first))
            {
                BOOST_LOG_TRIVIAL(debug) << "After requiers entity";
                current_state->participents.push_back(new_participent);
            }
        }
        else
        {
            should_accept &= new_participent.first == current_state->participents[i].first;
            should_accept &= new_participent.second == current_state->participents[i].second;
        };
    };
    return should_accept;
}

void Administrator::respond_game_invite_acceptance(const channel_ptr &channel,
                                                   GameInvite *current_game_state,
                                                   GameInvite *request,
                                                   const bool should_accept) const
{
    if (should_accept)
    {
        BOOST_LOG_TRIVIAL(info) << "Accepting Entity to the game";
        current_game_state->intend = GameInviteIntend::Acknowledge;
        // Send the player an Acknowledge
        send_game_state(channel, *current_game_state);
        BOOST_LOG_TRIVIAL(debug) << "Number of players reqeusted: " << current_game_state->number_of_players;
        BOOST_LOG_TRIVIAL(debug) << "Current number of players + refs accepted: " << current_game_state->participents.size();
        BOOST_LOG_TRIVIAL(debug) << "current_game_state->has_not_enough_participents()" << current_game_state->has_enough_participents();
        if (current_game_state->has_enough_participents())
        {
            current_game_state->intend = GameInviteIntend::Starting;
        }
        else
        {
            current_game_state->intend = GameInviteIntend::Requesting;
        }
    }
    else
    {
        request->intend = GameInviteIntend::Reject;
        BOOST_LOG_TRIVIAL(info) << "Sending rejection";
        // Request is invalid
        send_game_state(channel, *request);
    }
    // Ask for new players or give the start signal
    send_game_state(channel, *current_game_state);
}

void Administrator::listnen_for_participants(const channel_ptr &channel,
                                             const std::string &consumer,
                                             GameInvite *accepted_game_state) const
{
    auto envelope = AmqpClient::Envelope::ptr_t();

    int timeout_number = 5;
    do
    {
        bool has_found_message = channel->BasicConsumeMessage(consumer, envelope, 500);

        if (has_found_message)
        {
            BOOST_LOG_TRIVIAL(debug) << "has_found_message";
            GameInvite game_state = GameInvite::from_json(envelope->Message()->Body());

            if (game_state.intend == GameInviteIntend::Accepting)
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
             accepted_game_state->intend != GameInviteIntend::Starting);
}