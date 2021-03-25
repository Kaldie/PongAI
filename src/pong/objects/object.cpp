#define BOOST_GEOMETRY_DISABLE_DEPRECATED_03_WARNING
#include <boost/geometry.hpp>
#include <boost/property_tree/ptree.hpp>

#include <PongAI/pong/objects/object.hpp>

namespace pong::objects
{

    void update_ptree(const Point2D &point,
                      boost::property_tree::ptree *tree)
    {
        tree->put("x", point.x());
        tree->put("y", point.x());
    }

    Point2D from_ptree(const boost::property_tree::ptree &ptree)
    {
        return Point2D(ptree.get<double>("x"), ptree.get<double>("y"));
    }

}