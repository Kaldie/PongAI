#define BOOST_GEOMETRY_DISABLE_DEPRECATED_03_WARNING
#include <boost/geometry.hpp>
#include <boost/property_tree/ptree.hpp>

#include <PongAI/pong/objects/object.hpp>

namespace pong::objects
{

    Object::Object(const std::string &id, const std::string &owner)
        : id{id}, owner{owner}
    {
    }

    Object::Object()
        : Object("unknown", "unkown") {}

    Object::Object(const boost::property_tree::ptree &tree)
        : id{tree.get<std::string>("id")},
          owner{tree.get<std::string>("owner")}
    {
    }

    boost::property_tree::ptree Object::to_ptree() const
    {
        boost::property_tree::ptree ptree;
        ptree.put("owner", owner);
        ptree.put("id", id);
        return ptree;
    }

    void Object::set_owner(const std::string &owner)
    {
        this->owner = owner;
    }

    void Object::set_id(const std::string &id)
    {
        this->id = id;
    }

    std::string Object::get_id() const
    {
        return id;
    }

    std::string Object::get_owner() const
    {
        return owner;
    }

    boost::property_tree::ptree to_ptree(const Point2D &point)
    {
        boost::property_tree::ptree tree;
        tree.put("x", point.x());
        tree.put("y", point.x());
        return tree;
    }

    Point2D from_ptree(const boost::property_tree::ptree &ptree)
    {
        return Point2D(ptree.get<double>("x"), ptree.get<double>("y"));
    }

}