#include <PongAI/pong/location.hpp>
#include <PongAI/pong/objects/ball.hpp>
#include <PongAI/pong/objects/field.hpp>
#include <PongAI/pong/objects/goal.hpp>
#include <PongAI/pong/objects/paddle.hpp>
#include <PongAI/util.hpp>

#include <cmath>
#include <vector>

namespace pong::objects
{

    Field::Field(const double &height, const double &width)
        : Object{},
          height{height}, width{width},
          polygon{{{0, 0},          // upper left
                   {width, 0},      // upper right
                   {width, height}, // lower right
                   {0, height},     // lower left
                   {0, 0}}},        // upper left, closing the polygon
          goals{
              {
                  // goal 1
                  {width / 2, 0}, // location
                  width * 0.3,    // width
                  2,              // height
              },
              {
                  // goal 2
                  {width / 2, height}, // location
                  width * 0.3,         // width
                  2,                   // height
              },
          }
    {
    }

    Field::Field()
        : Field(50, 300)
    {
    }

    Field::Field(const boost::property_tree::ptree &ptree)
    {
        height = ptree.get<int>("height");
        width = ptree.get<int>("width");

        polygon = Polygon2D({{{0, 0}, {width, 0}, {width, height}, {0, height}, {0, 0}}});

        auto paddles = ptree.get_child_optional("paddles");
        auto balls = ptree.get_child_optional("balls");
        auto goals = ptree.get_child_optional("goals");

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

        if (goals.has_value())
        {
            for (auto goal : goals.value())
            {
                this->goals.push_back(Goal(goal.second));
            }
        }
    }

    void Field::add_ball(const Ball &ball)
    {
        Ball aBall = ball;
        if (ball.get_location().x() == 0.0 &&
            ball.get_location().y() == 0)
        {
            aBall.move({get_random_integer(0, width),
                        get_random_integer(0, height)});
            aBall.set_angle(get_random_integer(0, 360) * (2 * M_PI / 360.0));
        }
        balls.push_back(aBall);
    }

    void Field::add_paddle(const Paddle &paddle)
    {
        // hacking this in, see what comes out after some refactor
        bool need_goal = true;
        int i = 0;
        while (need_goal && i < goals.size())
        {
            Goal goal = goals[i];
            if (goal.get_owner() == "unkown")
            {
                goal.set_owner(paddle.get_owner());
                need_goal = false;
            }
            ++i;
        }
        assert(!need_goal);
        paddles.push_back(paddle);
    }

    boost::property_tree::ptree Field::to_ptree() const
    {
        boost::property_tree::ptree ptree;
        ptree.put("width", width);
        ptree.put("height", height);

        // todo: #3 refactor this stuff out of here!
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

        if (goals.size() > 0)
        {
            boost::property_tree::ptree goal_ptree;
            for (auto goal : goals)
            {
                goal_ptree.add_child("", goal.to_ptree());
            }
            ptree.add_child("goals", goal_ptree);
        }

        return ptree;
    };

} // namespace pong
