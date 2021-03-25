#ifndef __PADDLE_H__
#define __PADDLE_H__

#include <PongAI/default_include.hpp>
#include <PongAI/pong/objects/object.hpp>

namespace pong::objects
{
    class Paddle: Object
    {
    private:
        double width;
        double height;
        Point2D center;
        Polygon2D polygon;

        void generate_polygon();
    public:
        Paddle();
        Paddle(const Point2D& center, const double& width, const double& height);
        Paddle(const boost::property_tree::ptree& ptree);

        void move(const double& time);

        virtual boost::property_tree::ptree to_ptree() const;

    };
} // namespace pong::objects

#endif // __PADDLE_H__