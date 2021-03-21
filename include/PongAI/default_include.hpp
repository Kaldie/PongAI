#ifndef __DEFAULT_INCLUDE_H__
#define __DEFAULT_INCLUDE_H__

#include <boost/shared_ptr.hpp>
#include <PongAI/util.hpp>

class Location;
typedef boost::shared_ptr<Location> Location_ptr;

class Paddle;
typedef boost::shared_ptr<Paddle> Paddle_ptr;

class Player;
typedef boost::shared_ptr<Player> Player_ptr;

class Ball;
typedef boost::shared_ptr<Ball> Ball_ptr;

class Field;
typedef boost::shared_ptr<Field> Field_ptr;

enum FieldSize
{
    Small,
    Medium,
    Large,
};

enum GameStateIntend {
    Requesting,
    Accepting,
    Acknowledge,
    Reject,
    Starting,
    Running,
    Stale,
};

enum EntityType {
    Entity,
    AdministratorType,
    RefereeType,
    PlayerType
};

#endif // __DEFAULT_INCLUDE_H__