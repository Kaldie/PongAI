#include <PongAI/pong/location.hpp>
#include <PongAI/pong/objects/goal.hpp>
#include <boost/make_shared.hpp>

namespace pong::objects
{
    Goal::Goal(const Point2D &center, const double &width,
                   const double &height)
        : center{center}, width{width}, height{height}
    {
        generate_polygon();
    }

    Goal::Goal()
        : Goal({0.0, 0.0}, // location
                 10,         // width
                 5)          // height)
    {
    }

    Goal::Goal(const boost::property_tree::ptree &ptree)
        : Object{ptree.get_child("object")},
          center{from_ptree(ptree.get_child("center"))},
          width{ptree.get<double>("width")},
          height{ptree.get<double>("height")}
    {
        generate_polygon();
    }

    void Goal::generate_polygon()
    {
        generate_polygon(center, &polygon);
    }

    void Goal::generate_polygon(const Point2D &location, Polygon2D *polygon) const
    {
        double half_width = 0.5 * width;
        double half_height = 0.5 * height;

        std::vector<Point2D> new_polygon({{location.x() - half_width, location.y() - half_height},
                                          {location.x() - half_width, location.y() + half_height},
                                          {location.x() + half_width, location.y() + half_height},
                                          {location.x() + half_width, location.y() - half_height},
                                          {location.x() - half_width, location.y() - half_height}});

        polygon->outer().swap(new_polygon);
    }

    boost::property_tree::ptree Goal::to_ptree() const
    {
        boost::property_tree::ptree ptree;
        ptree.put("width", width);
        ptree.put("height", height);

        ptree.add_child("center", pong::objects::to_ptree(center));
        ptree.add_child("object", Object::to_ptree());
        return ptree;
    }

    void Goal::move(const Point2D &new_location)
    {
        center = new_location;
    }
} // namespace pong::objects
