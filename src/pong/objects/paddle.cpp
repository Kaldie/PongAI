#include <PongAI/pong/objects/paddle.hpp>
#include <PongAI/pong/location.hpp>
#include <boost/make_shared.hpp>

Paddle::Paddle()
{
    this->location = boost::make_shared<Location>();
    this->width = 1;
}