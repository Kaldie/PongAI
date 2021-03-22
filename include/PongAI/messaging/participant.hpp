#ifndef __PARTICIPANT_H__
#define __PARTICIPANT_H__

#include "entity.hpp"

namespace messaging
{

    class Participant : Entity
    {
    private:
        virtual std::string entity_type() const override { return "Player"; };
        void listen_and_accept_game_invite(const channel_ptr &channel,
                                           const std::string &consumer) const;

        bool has_acknowledged_invite(const channel_ptr &channel,
                                     const std::string &consumer) const;

    public:
        void find_and_participate();

        Participant(){};
        ~Participant(){};
    };
};
#endif // __PARTICIPANT_H__