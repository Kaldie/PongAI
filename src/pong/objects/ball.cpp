#include <PongAI/pong/objects/ball.hpp>
#include <boost/make_shared.hpp>
#include <PongAI/pong/location.hpp>

Ball::Ball() 
{
    location = boost::make_shared<Location>();    
}