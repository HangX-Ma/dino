#ifndef DINO_UTILS_H
#define DINO_UTILS_H

#include "SDL2/SDL_timer.h"
#include "lgfx/v1/misc/enum.hpp"
#include <string>

namespace dino
{

class Utils
{
public:
    static uint32_t getTimestamp() { return SDL_GetTicks64(); }

    /** convert enum to underlying type */
    template <typename Type>
    static constexpr auto toIndex(Type val) -> decltype(auto)
    {
        return static_cast<std::common_type_t<int, std::underlying_type_t<Type>>>(val);
    }
};

enum class Action
{
    NOTHING,
    JUMP,
    BEND_OVER,
    RESTART,
};
const std::string DINO_ACTION_STR[] = {"NOTHING", "JUMP", "BEND_OVER", "RESTART"};

using RenderConfig_t = struct RenderConfig
{
    int32_t screen_width;
    int32_t screen_height;
    double padding_ratio; // range(0, 1.0)
    int32_t last_ts;      // timestamp

    uint16_t game_speed;
    uint16_t max_game_speed;
    uint16_t update_interval;
    uint16_t minimum_interval;

    uint32_t score_diff;
    uint32_t bird_come_score;

    int background_color;
    int prev_background_color;

    int32_t getPaddingHeight() { return screen_height * padding_ratio; }
    int32_t getMiddlePaddingY() { return screen_height * padding_ratio; }
    int32_t getMiddlePaddingHeight() { return screen_height * (1.0 - 2.0 * padding_ratio); }
    int32_t getBottomPaddingY() { return screen_height * (1.0 - padding_ratio); }

    lgfx::color_depth_t depth;
    double zoom_x;
    double zoom_y;
    // Y is related with zoom_y, if you change zoom_y, you need to change `getGroundY` and
    // `getJumpApexY`
    int32_t getGroundY() { return getBottomPaddingY() + getMiddlePaddingHeight() * 0.2; }
    int32_t getJumpApexY() { return getBottomPaddingY() - getMiddlePaddingHeight() * 0.5; }
};

using Position_t = struct Position
{
    int32_t x;
    int32_t y;
};

} // namespace dino

#endif