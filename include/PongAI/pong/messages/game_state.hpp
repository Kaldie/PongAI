#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__
#include <PongAI/pong/messages/action.hpp>
#include <PongAI/pong/objects/field.hpp>
#include <string>

namespace pong
{

    namespace messages
    {

        class GameState
        {

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


            Actions get_actions() const { return actions; };
            bool add_action(const Action &action, const bool override = false);

            std::vector<std::string> get_players() const { return players; };
        };

    } // namespace messages
}

#endif // __GAME_STATE_H__