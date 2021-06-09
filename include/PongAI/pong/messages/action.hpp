#ifndef __ACTION_H__
#define __ACTION_H__
#include <utility>

namespace pong
{
    enum Direction
    {
        left,
        right
    };

    typedef std::pair<Direction, float> Action;

} // namespace pong

#endif // __ACTION_H__