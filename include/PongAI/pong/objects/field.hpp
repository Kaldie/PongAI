#ifndef __FIELD_H__
#define __FIELD_H__

#include <PongAI/default_include.hpp>
#include <PongAI/pong/objects/object.hpp>
#include <vector>

namespace pong::objects
{
    class Goal;
    class Paddle;
    class Ball;

    class Field : public Object
    {
    private:
        double height = 100;
        double width = 100;

        std::vector<Goal> goals = {};
        std::vector<Paddle> paddles = {};
        std::vector<Ball> balls = {};

        Polygon2D polygon;

    public:
        Field();
        Field(const boost::property_tree::ptree &ptree);

        Field(const double &height, const double &width);

        ~Field(){};

        void add_ball(const Ball& ball);
        void add_paddle(const Paddle& paddle);

        virtual boost::property_tree::ptree to_ptree() const override;
    };
} // namespace objects

#endif // __FIELD_H__