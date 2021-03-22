#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <PongAI/messaging/messages/game_invite.hpp>
#include <boost/shared_ptr.hpp>

namespace messaging
{
    namespace messages
    {
        typedef boost::shared_ptr<GameState> GameState_ptr;
        
        class GameState: GameInvite
        {};
    };
};
#endif // __GAME_STATE_H__