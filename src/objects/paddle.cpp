#include <PongAI/paddle.hpp>
#include <PongAI/location.hpp>
#include <boost/make_shared.hpp>

Paddle::Paddle()
{
    this->location = boost::make_shared<Location>();
    this->width = 1;
}