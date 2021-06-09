#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__
#include <PongAI/pong/objects/field.hpp>
#include <PongAI/pong/messages/action.hpp>
#include <string>

namespace pong
{

    namespace messages
    {


        class GameState
        {

        typedef std::vector<std::pair<std::string, pong::Action>> Actions;

        private:
            std::string game_id;
            pong::objects::Field field;
            std::vector<std::string> players;
            Actions actions;

        public:
            GameState();
            GameState(const std::string &id, const pong::objects::Field &field);
            GameState(const pong::objects::Field &field);

            static GameState from_json(const std::string &json_string);
            static std::string to_json(const GameState game_state);

            void set_field(const pong::objects::Field &field);
            pong::objects::Field get_field() const;

            void set_game_id(const std::string &id);
            std::string get_game_id() const;

            Actions get_actions() const { return actions;};
        };

    } // namespace messages
}

#endif // __GAME_STATE_H__