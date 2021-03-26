#include <PongAI/pong/location.hpp>
#include <PongAI/pong/objects/paddle.hpp>
#include <boost/make_shared.hpp>

namespace pong::objects
{
    Paddle::Paddle(const Point2D &center, const double &width,
                   const double &height, const double &speed)
        : center{center}, width{width}, height{height}, speed{speed}
    {
        generate_polygon();
    }

    Paddle::Paddle()
        : Paddle({0.0, 0.0}, // location
                 10,         // width
                 5,          // height
                 5)          // speed
    {
    }

    Paddle::Paddle(const boost::property_tree::ptree &ptree)
        : Object{ptree.get_child("object")},
          center{from_ptree(ptree.get_child("center"))},
          width{ptree.get<double>("width")},
          height{ptree.get<double>("height")},
          speed{ptree.get<double>("speed")}
    {
        generate_polygon();
    }

    void Paddle::generate_polygon()
    {
        generate_polygon(center, &polygon);
    }

    void Paddle::generate_polygon(const Point2D &location, Polygon2D *polygon) const
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

    boost::property_tree::ptree Paddle::to_ptree() const
    {
        boost::property_tree::ptree ptree;
        ptree.put("width", width);
        ptree.put("height", height);

        ptree.add_child("center", pong::objects::to_ptree(center));
        ptree.add_child("object", Object::to_ptree());
        return ptree;
    }

    Polygon2D Paddle::predict_polygon(const double &time) const
    {
        Polygon2D polygon_prediction;
        Point2D new_location(center.x() + time * speed, center.y());
        generate_polygon(new_location, &polygon_prediction);
        return polygon_prediction;
    }

    void Paddle::move(const Point2D &new_location)
    {
        center = new_location;
    }
} // namespace pong::objects
