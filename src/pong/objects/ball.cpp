#include <PongAI/pong/location.hpp>
#include <PongAI/pong/objects/ball.hpp>
#include <boost/geometry.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <math.h>

namespace pong::objects
{
    Ball::Ball(const double &speed, const Point2D &location)
        : location{location}, speed{speed}
    {
    }

    Ball::Ball(const double &speed)
        : Ball(speed, Point2D(0.0, 0.0))
    {
    }

    Ball::Ball()
        : Ball(5)
    {
    }

    Ball::Ball(const boost::property_tree::ptree &ptree)
        : Object{ptree.get_child("object")},
          location{from_ptree(ptree.get_child("location"))},
          angle{ptree.get<double>("angle")},
          speed{ptree.get<double>("speed")}
    {
    }

    boost::property_tree::ptree Ball::to_ptree() const
    {
        boost::property_tree::ptree ptree;
        ptree.put("angle", angle);
        ptree.put("speed", speed);

        ptree.add_child("location", pong::objects::to_ptree(location));
        ptree.add_child("object", Object::to_ptree());
        return ptree;
    }

    void Ball::increase_speed(const double &increase)
    {
        speed += increase;
    }

    void Ball::decrease_speed(const double &decrease)
    {
        increase_speed(-1 * abs(decrease));
    }

    Line2D Ball::predict_movement(const double &time)
    {
        return Line2D(location,
                      Point2D(location.x() + sin(angle) * time * speed,
                              location.y() + cos(angle) * time * speed));
    }

    void Ball::move(const Point2D &location)
    {
        this->location = location;
    }
}