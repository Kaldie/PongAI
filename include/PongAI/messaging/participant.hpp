#ifndef __PARTICIPANT_H__
#define __PARTICIPANT_H__

#include "entity.hpp"
#include <boost/shared_ptr.hpp>
#include <string>

namespace AmqpClient
{

    class Envelope;
    typedef boost::shared_ptr<Envelope> Envelope_ptr;

}

namespace messaging
{
    class Participant : Entity
    {
    private:
        // TODO: #1 Remove this!!!
        virtual std::string entity_type() const override { return "Player"; };

        void listen_and_accept_game_invite(const channel_ptr &channel,
                                           const std::string &consumer) const;

        bool has_acknowledged_invite(const channel_ptr &channel,
                                     const std::string &consumer) const;

        virtual std::string prepare_for_game(
            const channel_ptr &channel,
            const messages::GameInvite &game_invite) const;

        virtual bool listen_for_game_message(
            const channel_ptr &channel,
            const std::string &consumer,
            AmqpClient::Envelope_ptr &envelope) const;

        virtual std::string respond_on_game_message(
            const std::string &game_message);

        void send_message(
            const channel_ptr &channel,
            const messages::GameInvite &invite,
            const std::string &message) const;

    public:
        void find_and_accept();
        void participate(const messages::GameInvite &game_invite);

        Participant(){};
        ~Participant(){};
    };
};
#endif // __PARTICIPANT_H__