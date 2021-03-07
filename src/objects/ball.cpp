#include <PongAI/ball.hpp>
#include <boost/make_shared.hpp>
#include <PongAI/location.hpp>

Ball::Ball() 
{
    location = boost::make_shared<Location>();    
}