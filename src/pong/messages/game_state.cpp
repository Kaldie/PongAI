#include <PongAI/pong/messages/game_state.hpp>
#include <PongAI/pong/objects/field.hpp>
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

        GameState::GameState(){
            field = boost::make_shared<objects::Field>();
        }

        GameState GameState::from_json(const std::string &input)
        {
            GameState game_state;
            std::stringstream ss;
            boost::property_tree::ptree out;
            ss << input;
            boost::property_tree::read_json(ss, out);

            game_state.field = boost::make_shared<objects::Field>(
                objects::Field(out.get_child("gamestate.field")));

            return game_state;
        }

        std::string GameState::to_json(const GameState game_state)
        {
            boost::property_tree::ptree out;
            out.add_child("gamestate.field", game_state.field->to_ptree());
            std::ostringstream oss;
            boost::property_tree::write_json(oss, out);
            return oss.str();
        }

    } // namespace messages
} // namespace pong
