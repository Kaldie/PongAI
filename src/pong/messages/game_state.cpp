#include <PongAI/pong/messages/game_state.hpp>
#include <PongAI/pong/objects/ball.hpp>
#include <PongAI/pong/objects/field.hpp>
#include <PongAI/pong/objects/goal.hpp>
#include <PongAI/pong/objects/paddle.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#define BOOST_BIND_GLOBAL_PLACEHOLDERS // can be removed once this is fixed: https://github.com/boostorg/property_tree/pull/50
#include <boost/property_tree/json_parser.hpp>
#undef BOOST_BIND_GLOBAL_PLACEHOLDERS

namespace pong
{
    namespace messages
    {

        GameState::GameState()
        {
        }

        GameState::GameState(const pong::objects::Field &field)
            : field(field)
        {
        }

        GameState::GameState(const std::string &id, const pong::objects::Field &field)
            : field(field), game_id(id)
        {
        }

        GameState GameState::from_json(const std::string &input)
        {
            GameState game_state;
            std::stringstream ss;
            boost::property_tree::ptree out;
            ss << input;
            boost::property_tree::read_json(ss, out);
            game_state.game_id = out.get<std::string>("id");
            game_state.field = objects::Field(out.get_child("gamestate.field"));

            auto optional_actions_ptree = out.get_child_optional("actions");
            if (optional_actions_ptree.has_value())
            {
                for (auto ptree : optional_actions_ptree.value())
                {
                    game_state.add_action(Action(ptree.second));
                }
            }

            return game_state;
        }

        std::string GameState::to_json(const GameState game_state)
        {
            boost::property_tree::ptree out;
            out.put("id", game_state.game_id);
            out.add_child("gamestate.field", game_state.field.to_ptree());

            boost::property_tree::ptree actions_ptree;
            for (auto action : game_state.actions)
            {
                actions_ptree.add_child(action.first, Action::to_ptree(action.second));
            }
            out.add_child("actions", actions_ptree);

            std::ostringstream oss;
            boost::property_tree::write_json(oss, out);
            return oss.str();
        }

        void GameState::set_field(const pong::objects::Field &field)
        {
            this->field = field;
        }

        objects::Field GameState::get_field() const
        {
            return field;
        }

        void GameState::set_game_id(const std::string &id)
        {
            game_id = id;
        }

        std::string GameState::get_game_id() const
        {
            return game_id;
        }

        bool GameState::add_action(const Action &action, const bool overwrite)
        {
            auto return_value = actions.insert({action.user_id, action});
            if (not return_value.second && overwrite) {
                actions[action.user_id] = action;
                return_value.second = true;
            }
            return return_value.second;
        }

    } // namespace messages
} // namespace pong
