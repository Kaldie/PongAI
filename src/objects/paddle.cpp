#include <PongAI/paddle.hpp>
#include <PongAI/location.hpp>
#include <boost/make_shared.hpp>


Paddle::Paddle() {
    location = boost::make_shared<Location>();
}