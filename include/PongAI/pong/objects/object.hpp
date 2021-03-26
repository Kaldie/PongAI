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
            std::string id;
            std::string owner;

        public:
            Object();
            Object(const std::string& id, const std::string& owner);
            Object(const boost::property_tree::ptree& tree);
            virtual ~Object(){};

            void set_owner(const std::string& owner);
            std::string get_owner() const;
            void set_id(const std::string& id);
            std::string get_id() const;

            virtual boost::property_tree::ptree to_ptree() const;
        };

        boost::property_tree::ptree to_ptree(const Point2D &point);

        Point2D from_ptree(const boost::property_tree::ptree &ptree);
    };
} // namespace pong

#endif // __OBJECT_H__