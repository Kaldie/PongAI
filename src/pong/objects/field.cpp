#include <PongAI/pong/objects/field.hpp>
#include <PongAI/pong/location.hpp>

#include <cmath>
#include <vector>


double Field::speed()
{
    return default_speed + pow(speed_multiplier, current_turn);
}

bool Field::isInside(Location_ptr location)
{
    if (location->x < 0 || location->y < 0)
    {
        return false;
    }

    if (location->x > width || location->y > length)
    {
        return false;
    }
    return true;
}


void Field::add_ball(Ball_ptr ball) 
{
    balls.push_back(ball);
}

void Field::add_paddle(Paddle_ptr paddle) 
{
    paddles.push_back(paddle);
}
