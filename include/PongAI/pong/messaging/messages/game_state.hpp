#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <PongAI/messaging/messages/game_invite.hpp>
#include <unordered_map>



namespace pong
{
        namespace messages
        {


            class GameState : messaging::messages::GameInvite {

                private:
                    std::unordered_map<std::string, 

            };
        } // namespace messages
           
} // namespace ping

#endif // __GAME_STATE_H__