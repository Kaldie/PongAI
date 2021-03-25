#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <PongAI/messaging/messages/game_invite.hpp>
#include <string>
#include <unordered_map>

namespace boost
{
    template <typename T>
    class shared_ptr;
} // namespace boost

namespace pong::objects
{
    class Object;
    class Field;
    typedef boost::shared_ptr<Field> Field_ptr;
}

namespace pong::messages
{

    class GameState : messaging::messages::GameInvite
    {

    private:
        pong::objects::Field_ptr field;

    public:
        GameState();

        static GameState from_json(const std::string &json_string);
        static std::string to_json(const GameState game_state);
    };
} // namespace messages

#endif // __GAME_STATE_H__