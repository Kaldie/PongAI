#ifndef __REFEREE_H__
#define __REFEREE_H__

#include <PongAI/messaging/participant.hpp>

namespace pong
{
    class Referee : messaging::Participant
    {
    private:
        virtual std::string entity_type() const override { return "Referee"; };
    };
};
#endif // __REFEREE_H__