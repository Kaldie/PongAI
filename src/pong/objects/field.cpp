#include <PongAI/pong/location.hpp>
#include <PongAI/pong/objects/field.hpp>
#include <PongAI/pong/objects/paddle.hpp>

#include <cmath>
#include <vector>

namespace pong::objects
{

    Field::Field(const double &height, const double &width)
        : Object{},
          height{height}, width{width},
          polygon{{{0, 0}, {width, 0}, {width, height}, {0, height}, {0, 0}}}
    {
    }

    Field::Field()
        : Field(100, 100)
    {
    }

    Field::Field(const boost::property_tree::ptree &ptree)
    {
        height = ptree.get<int>("height");
        width = ptree.get<int>("width");

        polygon = Polygon2D({{{0, 0}, {width, 0}, {width, height}, {0, height}, {0, 0}}});

        auto paddles = ptree.get_child_optional("paddles");
        auto balls = ptree.get_child_optional("balls");

        if (paddles.has_value())
        {
            for (auto paddle : paddles.value())
            {
                this->add_paddle(Paddle(paddle.second));
            }
        }

        if (balls.has_value())
        {
            for (auto ball : balls.value())
            {
                this->add_ball(Ball(ball.second));
            }
        }
    }

    void Field::add_ball(Ball ball)
    {
        balls.push_back(ball);
    }

    void Field::add_paddle(Paddle paddle)
    {
        paddles.push_back(paddle);
    }

    boost::property_tree::ptree Field::to_ptree() const
    {
        boost::property_tree::ptree ptree;
        ptree.put("width", width);
        ptree.put("height", height);

        if (paddles.size() > 0)
        {
            boost::property_tree::ptree paddles_ptree;
            for (auto paddle : paddles)
            {
                paddles_ptree.add_child("", paddle.to_ptree());
            }
            ptree.add_child("paddles", paddles_ptree);
        }

        if (balls.size() > 0)
        {
            boost::property_tree::ptree balls_ptree;
            for (auto ball : balls)
            {
                balls_ptree.add_child("", ball.to_ptree());
            }
            ptree.add_child("balls", balls_ptree);
        }
        return ptree;
    };

} // namespace pong
