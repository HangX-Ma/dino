#ifndef INCLUDE_DINO_UTILS_H_
#define INCLUDE_DINO_UTILS_H_

#include <algorithm>
#include "SDL2/SDL_timer.h"

namespace dino
{

using Position_t = struct Position
{
    int32_t x;
    int32_t y;
};

using BoundingBox_t = struct BoundingBox
{
    Position upper_left;   // min (x,y)
    Position lower_right;  // max (x,y)
};

class Utils
{
 public:
    static uint32_t getTimestamp() { return SDL_GetTicks64(); }

    // convert enum to underlying type
    template <typename Type>
    static constexpr auto toIndex(Type val) -> decltype(auto)
    {
        return static_cast<std::common_type_t<int, std::underlying_type_t<Type>>>(val);
    }

    // AABB bounding box collision checker
    static bool intersects(BoundingBox_t box1, BoundingBox_t box2)
    {
        int32_t inter_left = std::max(box1.upper_left.x, box2.upper_left.x);
        int32_t inter_top = std::max(box1.upper_left.y, box2.upper_left.y);
        int32_t inter_right = std::min(box1.lower_right.x, box2.lower_right.x);
        int32_t inter_bottom = std::min(box1.lower_right.y, box2.lower_right.y);

        return (inter_left < inter_right) && (inter_top < inter_bottom);
    }
};

}  // namespace dino

#endif  // INCLUDE_DINO_UTILS_H_
