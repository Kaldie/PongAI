#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.hpp"
#include <boost/shared_ptr.hpp>

class Paddle;
class Ball;

class Player : Entity
{
private:
    
    std::string identifier;
    boost::shared_ptr<Paddle> paddle;
    boost::shared_ptr<Ball> ball;

public:

    virtual std::string get_entity_type() const override { return "Player";};
};

#endif // __PLAYER_H__