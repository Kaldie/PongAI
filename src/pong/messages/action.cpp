#include <PongAI/pong/messages/action.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#define BOOST_BIND_GLOBAL_PLACEHOLDERS // can be removed once this is fixed: https://github.com/boostorg/property_tree/pull/50
#include <boost/property_tree/json_parser.hpp>
#undef BOOST_BIND_GLOBAL_PLACEHOLDERS

namespace pong
{
    /**
     * @brief Construct a new Action:: Action object
     * 
     */
    Action::Action()
    {
    }

    /**
     * @brief Construct a new Action:: Action object
     * 
     * @param tree const boost::property_tree::ptree& 
     */
    Action::Action(const boost::property_tree::ptree &tree)
    {
        force = tree.get<double>("force");
        direction = from_string(tree.get<std::string>("direction"));
        user_id = tree.get<std::string>("user");
        object_id = tree.get<std::string>("object");
    }

    /**
     * @brief convert an action to a json string
     * 
     * @param action 
     * @return std::string 
     */
    std::string Action::to_json(const Action &action)
    {
        auto out = to_ptree(action);
        std::ostringstream oss;
        boost::property_tree::write_json(oss, out);
        return oss.str();
    }

    /**
     * @brief Create a property tree from an action
     * 
     * @param action 
     * @return boost::property_tree::ptree 
     */
    boost::property_tree::ptree Action::to_ptree(const Action &action)
    {
        boost::property_tree::ptree out;
        out.put("force", action.force);
        out.put("direction", pong::to_string(action.direction));
        out.put("object", action.object_id);
        out.put("user", action.user_id);
        return out;
    }

    /**
     * @brief creates a action from a json string
     * 
     * @param json_string 
     * @return Action 
     */
    Action Action::from_json(const std::string &json_string)
    {
        std::stringstream ss;
        boost::property_tree::ptree out;
        ss << json_string;
        boost::property_tree::read_json(ss, out);
        return Action(out);
    }

    Direction from_string(const std::string &input)
    {

        if (input == "left")
            return Direction::left;
        else if (input == "right")
            return Direction::right;
        else
            throw "Unknown direction";
    }

    std::string to_string(const Direction &input)
    {
        switch (input)
        {
        case Direction::left:
        {
            return "left";
        }
        case Direction::right:
        {
            return "right";
        }
        default:{
            throw "Unknown direction";
        }
        }
    }

} // namespace pong