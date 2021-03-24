#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <PongAI/messaging/messages/game_invite.hpp>
#include <boost/shared_ptr.hpp>

namespace messaging
{
    namespace messages
    {
        
        class GameState: GameInvite{};
        typedef boost::shared_ptr<GameState> GameState_ptr;
    };
};
#endif // __GAME_STATE_H__