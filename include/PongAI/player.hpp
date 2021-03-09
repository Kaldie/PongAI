#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.hpp"
#include <boost/shared_ptr.hpp>

class Paddle;
class Ball;

class Player : Entity
{
private:
    const static std::string entity_type;

    std::string identifier;
    boost::shared_ptr<Paddle> paddle;
    boost::shared_ptr<Ball> ball;

public:
};

#endif // __PLAYER_H__