#ifndef __PADDLE_H__
#define __PADDLE_H__

#include <PongAI/default_include.hpp>
#include <PongAI/pong/objects/object.hpp>

namespace pong::objects
{
    class Paddle : Object
    {
    private:
        double width;
        double height;
        double speed;
        Point2D center;
        Polygon2D polygon;

        void generate_polygon();
        void generate_polygon(const Point2D &location,
                              Polygon2D *polygon) const;

    public:
        Paddle();
        Paddle(const Point2D &center,
               const double &width,
               const double &height,
               const double &speed);

        Paddle(const boost::property_tree::ptree &ptree);
        virtual boost::property_tree::ptree to_ptree() const;

        Polygon2D predict_polygon(const double &time) const;
        void move(const Point2D& new_center);
    };
} // namespace pong::objects

#endif // __PADDLE_H__