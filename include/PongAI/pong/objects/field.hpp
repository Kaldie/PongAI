#ifndef __FIELD_H__
#define __FIELD_H__

#include <PongAI/default_include.hpp>
#include <PongAI/pong/objects/object.hpp>
#include <PongAI/pong/objects/ball.hpp>
#include <PongAI/pong/objects/paddle.hpp>
#include <vector>

namespace pong::objects
{

    class Field : Object
    {
    private:
        double height = 100;
        double width = 100;

        std::vector<Paddle> paddles = {};
        std::vector<Ball> balls = {};

        Polygon2D polygon;

    public:
        Field();
        Field(const boost::property_tree::ptree &ptree);

        Field(const double &height, const double &width);

        ~Field(){};

        void add_ball(Ball ball);
        void add_paddle(Paddle paddle);

        virtual boost::property_tree::ptree to_ptree() const override{};
    };
} // namespace objects

#endif // __FIELD_H__