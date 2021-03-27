#ifndef __BALL_H__
#define __BALL_H__

#include <PongAI/default_include.hpp>
#include <PongAI/pong/objects/object.hpp>

namespace pong
{
    namespace objects
    {
        class Ball : public Object
        {
        private:
            /* data */
            double angle; // in radians
            double speed;
            Point2D location;

        public:
            Ball(const double &speed, const Point2D &location);
            Ball(const double &speed);
            Ball();
            Ball(const boost::property_tree::ptree &tree);

            void increase_speed(const double &increase);
            void decrease_speed(const double &increase);

            void set_angle(const double &angle);
            void set_speed(const double &angle);

            Line2D predict_movement(const double &time_passed);
            Point2D get_location() const;

            void move(const Point2D &location);

            virtual boost::property_tree::ptree to_ptree() const override;
        };

    } // namespace objects

} // namespace pong

#endif // __BALL_H__