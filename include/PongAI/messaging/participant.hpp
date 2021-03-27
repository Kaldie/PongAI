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
     namespace messages
     {
         class GameInvite;
         typedef boost::shared_ptr<GameInvite> GameInvite_ptr;
     }

     using messages::GameInvite;
     using messages::GameInvite_ptr;

     class Participant : public Entity
     {
     private:
         // TODO: #1 Remove this!!!
         virtual std::string entity_type() const override { return "Player"; };

         GameInvite_ptr listen_and_accept_game_invite(const channel_ptr &channel,
                                                      const std::string &consumer) const;

         bool has_acknowledged_invite(const channel_ptr &channel,
                                      const std::string &consumer) const;

         void send_message(
             const channel_ptr &channel,
             const GameInvite &invite,
             const std::string &message) const;

         virtual void prepare_for_game(
             const GameInvite &game_invite) {};

         virtual std::string prepare_for_game(
             const channel_ptr &channel,
             const GameInvite &game_invite);

         virtual bool listen_for_game_message(
             const channel_ptr &channel,
             const std::string &consumer,
             AmqpClient::Envelope_ptr &envelope) const;

         virtual std::string respond_on_game_message(
             const std::string &game_message);

     public:
         GameInvite_ptr find_and_accept();
         void participate(const GameInvite &game_invite);

         Participant(){};
         ~Participant(){};
     };
 };
#endif // __PARTICIPANT_H__