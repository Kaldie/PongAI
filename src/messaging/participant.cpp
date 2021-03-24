#include <PongAI/messaging/messages/game_invite.hpp>
#include <PongAI/messaging/messages/game_state.hpp>
#include <PongAI/messaging/participant.hpp>
#include <PongAI/util.hpp>
#include <SimpleAmqpClient/Channel.h>
#include <boost/log/trivial.hpp>
#include <string>

namespace messaging
{

    GameInvite_ptr Participant::find_and_accept()
    {

        channel_ptr channel = create_channel();
        GameInvite_ptr game_invite;
        auto consumer = prepare_listen_exchange_topic(channel,
                                                      get_game_exchange_name(),
                                                      "game.#");
        do
        {
            BOOST_LOG_TRIVIAL(info) << "Listening for new game";
            game_invite = listen_and_accept_game_invite(channel, consumer);
        } while (!has_acknowledged_invite(channel, consumer));
        return game_invite;
    }

    bool Participant::has_acknowledged_invite(const channel_ptr &channel,
                                              const std::string &consumer) const
    {
        bool has_accepted(false);
        AmqpClient::Envelope::ptr_t envelope;
        bool has_seen_message = channel->BasicConsumeMessage(consumer, envelope, 500);
        if (has_seen_message)
        {
            messages::GameInvite game_invite = messages::GameInvite::from_json(envelope->Message()->Body());

            if ((game_invite.intend == GameInviteIntend::Acknowledge ||
                 game_invite.intend == GameInviteIntend::Reject) &&
                game_invite.participents.back().first == EntityType::PlayerType &&
                game_invite.participents.back().second == get_name())
            {
                has_accepted = game_invite.intend == GameInviteIntend::Acknowledge;
                BOOST_LOG_TRIVIAL(info) << "Has been accepted: " << has_accepted;
            }
            else
            {
                has_accepted = has_acknowledged_invite(channel, consumer);
            }
        }
        return has_accepted;
    }

    GameInvite_ptr Participant::listen_and_accept_game_invite(const channel_ptr &channel,
                                                              const std::string &consumer) const
    {
        GameInvite_ptr game_invite = boost::make_shared<messages::GameInvite>();
        AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer);
        (*game_invite) = messages::GameInvite::from_json(envelope->Message()->Body());

        if (game_invite->intend == GameInviteIntend::Requesting)
        {
            BOOST_LOG_TRIVIAL(info) << "Heared a game invite!";

            game_invite->participents.push_back(std::make_pair(
                EntityType::PlayerType, get_name()));

            game_invite->intend = GameInviteIntend::Accepting;

            BOOST_LOG_TRIVIAL(info) << "Sending an invite accept message";

            channel->BasicPublish(
                get_game_exchange_name(),
                game_invite->get_game_id(),
                AmqpClient::BasicMessage::Create(messages::GameInvite::to_json((*game_invite))));
        } else {
            game_invite.reset();
        }
        return game_invite;
    };

    std::string Participant::prepare_for_game(
        const channel_ptr &channel,
        const GameInvite_ptr &game_invite) const
    {
        return prepare_listen_exchange_topic(channel,
                                             get_game_exchange_name(),
                                             game_invite->get_game_id());
    }

    bool Participant::listen_for_game_message(
        const channel_ptr &channel,
        const std::string &consumer,
        AmqpClient::Envelope::ptr_t &envelope) const
    {
        envelope = channel->BasicConsumeMessage(consumer);
        return true;
    };

    void Participant::participate(const GameInvite_ptr &game_invite)
    {
        channel_ptr channel = create_channel();
        auto consumer = prepare_for_game(channel, game_invite);
        AmqpClient::Envelope_ptr envelope;
        std::string response;
        bool found_game_message(false);
        do
        {
            found_game_message = listen_for_game_message(
                channel, consumer, envelope);
            if (found_game_message)
            {
                response = respond_on_game_message(envelope->Message()->Body());
                send_message(channel, game_invite, response);
            }
        } while (found_game_message);
    }

    std::string Participant::respond_on_game_message(
        const std::string &game_message)
    {
        return game_message;
    }

    void Participant::send_message(
        const channel_ptr &channel,
        const GameInvite_ptr &invite,
        const std::string &message) const
    {
        channel->BasicPublish(get_game_exchange_name(),
                              invite->get_game_id(),
                              AmqpClient::BasicMessage::Create(message));
    }

};