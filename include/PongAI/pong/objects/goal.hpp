#ifndef __GOAL_H__
#define __GOAL_H__

#include <PongAI/default_include.hpp>
#include <PongAI/pong/objects/object.hpp>

namespace pong::objects
{
    class Goal : public Object
    {
    private:
        double width;
        double height;
        Point2D center;
        Polygon2D polygon;

        void generate_polygon();
        void generate_polygon(const Point2D &location,
                              Polygon2D *polygon) const;

    public:
        Goal();
        Goal(const Point2D &center,
               const double &width,
               const double &height);

        Goal(const boost::property_tree::ptree &ptree);
        virtual boost::property_tree::ptree to_ptree() const;
        void move(const Point2D& new_center);
    };
} // namespace pong::objects

#endif // __GOAL_H__