#include <PongAI/pong/location.hpp>
#include <PongAI/pong/objects/paddle.hpp>
#include <boost/make_shared.hpp>

namespace pong::objects
{
    Paddle::Paddle(const Point2D &center, const double &width, const double &height)
        : center{center}, width{width}, height{height}
    {
        generate_polygon();
    }

    Paddle::Paddle()
        : Paddle({0.0, 0.0}, 10, 5)
    {
    }

    Paddle::Paddle(const boost::property_tree::ptree &ptree)
    {
        width = ptree.get<double>("width");
        height = ptree.get<double>("height");
        center = from_ptree(ptree.get_child("center"));
        generate_polygon();
    }

    void Paddle::generate_polygon()
    {
        double half_width = 0.5 * width;
        double half_height = 0.5 * height;

        polygon = Polygon2D{{{center.x() - half_width, center.y() - half_height},
                             {center.x() - half_width, center.y() + half_height},
                             {center.x() + half_width, center.y() + half_height},
                             {center.x() + half_width, center.y() - half_height},
                             {center.x() - half_width, center.y() - half_height}}};
    }

    boost::property_tree::ptree Paddle::to_ptree() const
    {
        boost::property_tree::ptree ptree;
        ptree.put("width", width);
        ptree.put("height", height);

        boost::property_tree::ptree center_ptree;
        update_ptree(center, &center_ptree);
        ptree.add_child("center", center_ptree);
        return ptree;
    }

} // namespace pong::objects
