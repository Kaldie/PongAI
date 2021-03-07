#ifndef __FIELD_H__
#define __FIELD_H__

#include <PongAI/default_include.hpp>
#include <vector>

class Field
{
private:
    int length = 100;
    int width = 100;
    double default_speed = 5;
    double speed_multiplier = 1.005;
    int current_turn = 0;

    std::vector<Paddle_ptr> paddles = {};
    std::vector<Ball_ptr> balls = {};

public:
    Field();
    int turn() { return current_turn; };
    void set_speed_multiplier(double value) { speed_multiplier = value; };
    void end_turn() { ++current_turn; };

    double speed();
    bool isInside(Location_ptr);

    void add_ball(Ball_ptr);
    void add_paddle(Paddle_ptr);
};

#endif // __FIELD_H__