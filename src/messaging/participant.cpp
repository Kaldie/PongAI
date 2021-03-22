#include <PongAI/messaging/messages/game_invite.hpp>
#include <PongAI/messaging/messages/game_state.hpp>
#include <PongAI/messaging/participant.hpp>
#include <PongAI/util.hpp>
#include <SimpleAmqpClient/Channel.h>
#include <boost/log/trivial.hpp>
#include <string>

namespace messaging
{

    void Participant::find_and_accept()
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

    void Participant::listen_and_accept_game_invite(const channel_ptr &channel,
                                                    const std::string &consumer) const
    {
        using messages::GameInvite;
        AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer);
        GameInvite game_invite = GameInvite::from_json(envelope->Message()->Body());

        if (game_invite.intend == GameInviteIntend::Requesting)
        {
            BOOST_LOG_TRIVIAL(info) << "Heared a game invite!";

            game_invite.participents.push_back(std::make_pair(
                EntityType::PlayerType, get_name()));

            game_invite.intend = GameInviteIntend::Accepting;

            BOOST_LOG_TRIVIAL(info) << "Sending an invite accept message";

            channel->BasicPublish(
                get_game_exchange_name(),
                game_invite.get_game_id(),
                AmqpClient::BasicMessage::Create(GameInvite::to_json(game_invite)));
        }
    };

    std::string Participant::prepare_for_game(
        const channel_ptr &channel,
        const messages::GameInvite &game_invite) const
    {
        return prepare_listen_exchange_topic(channel,
                                             get_game_exchange_name(),
                                             game_invite.get_game_id());
    }

    void Participant::listen_for_game_message(
        const channel_ptr &channel,
        const std::string &consumer,
        AmqpClient::Envelope::ptr_t& envelope) {
            envelope = channel->BasicConsumeMessage(consumer);
    };

    void Participant::participate(const messages::GameInvite &game_invite)
    {
        channel_ptr channel = create_channel();
        auto consumer = prepare_for_game(channel, game_invite);
    }

};