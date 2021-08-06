#ifndef __ACTION_H__
#define __ACTION_H__

#include <utility>
#include <PongAI/pong/objects/object.hpp>
#include <boost/property_tree/ptree.hpp>
namespace pong
{
    enum Direction
    {
        left,
        right
    };

    const std::string direction_string[2]={"left","right"};
    
    std::string to_string(const Direction& input); 
    static Direction from_string(const std::string& input);

    class Action
    {

    public:
        Action();
        Action(const boost::property_tree::ptree& tree);

        double force;
        Direction direction;
        std::string object_id;
        std::string user_id;

        static Action from_json(const std::string &json_string);
        static std::string to_json(const Action& action);
                 
        static boost::property_tree::ptree to_ptree(const Action& action);


    };

   

    typedef std::map<std::string, Action> Actions;

} // namespace pong

#endif // __ACTION_H__