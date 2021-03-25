#ifndef __OBJECT_H__
#define __OBJECT_H__

#define BOOST_GEOMETRY_DISABLE_DEPRECATED_03_WARNING
#include <boost/geometry.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

namespace pong
{
    namespace objects
    {

        typedef boost::geometry::model::d2::point_xy<double> Point2D;
        typedef boost::geometry::model::segment<Point2D> Line2D;
        typedef boost::geometry::model::polygon<Point2D> Polygon2D;

        class Object
        {
        private:
            std::string id = "lala";

        public:
            Object(){};
            virtual ~Object(){};

            virtual boost::property_tree::ptree to_ptree() const = 0;
        };

        void update_ptree(const Point2D &point,
                          boost::property_tree::ptree *tree);

        Point2D from_ptree(const boost::property_tree::ptree &ptree);
    };
} // namespace pong

#endif // __OBJECT_H__