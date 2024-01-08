#ifndef DINO_UTILS_H
#define DINO_UTILS_H

#include <string>
#include "SDL2/SDL_timer.h"
#include "lgfx/v1/misc/enum.hpp"

namespace dino
{

static const std::string DINO_LOGO = R"(
 ________  .__               
 \______ \ |__| ____   ____  
  |    |  \|  |/    \ /  _ \ 
  |    `   \  |   |  (  <_> )
 /_______  /__|___|  /\____/ 
         \/        \/        
)";

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
    uint32_t score_skill_quick_drop;
    uint32_t score_skill_double_jump;
    uint32_t bird_come_score;
    uint32_t obstacle_max_delay;

    int background_color;
    int prev_background_color;

    int32_t getPaddingHeight() { return screen_height * padding_ratio; }
    int32_t getMiddlePaddingY() { return screen_height * padding_ratio; }
    int32_t getMiddlePaddingHeight() { return screen_height * (1.0 - 2.0 * padding_ratio); }
    int32_t getBottomPaddingY() { return screen_height * (1.0 - padding_ratio); }

    lgfx::color_depth_t depth;
    // The following configs are used for 'cloud' and 'ground' that don't need do collision
    // detection. If using the preprocessed image, the display quality of 'cloud' and 'ground' will
    // be bad!
    double zoom_x;
    double zoom_y;
    // Y is related with zoom_y, if you change zoom_y, you need to change `getGroundY`
    int32_t getGroundY() { return getBottomPaddingY() + getMiddlePaddingHeight() * 0.2; }
};

using Position_t = struct Position
{
    int32_t x;
    int32_t y;
};

using BoundingBox_t = struct BoundingBox
{
    Position upper_left;  // min (x,y)
    Position lower_right; // max (x,y)
};

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

    static bool intersects(BoundingBox_t box1, BoundingBox_t box2)
    {
        int32_t inter_left = std::max(box1.upper_left.x, box2.upper_left.x);
        int32_t inter_top = std::max(box1.upper_left.y, box2.upper_left.y);
        int32_t inter_right = std::min(box1.lower_right.x, box2.lower_right.x);
        int32_t inter_bottom = std::min(box1.lower_right.y, box2.lower_right.y);

        return (inter_left < inter_right) && (inter_top < inter_bottom);
    }

    static void showInfo()
    {
        /* Print logo */
        printf("\n%s"
               "\n - @author HangX-Ma\n"
               " - @version v1.0.0\n"
               " - @build at %s %s\n\n",
               DINO_LOGO.c_str(), __TIME__, __DATE__);
    }

    static void showUsage()
    {
        printf(">>> Use the following keys to escape cactus and bird!\n");
        printf(">>> - 'Up': Dino jump\n");
        printf(">>> - 'Down': Dino bend over\n");
        printf(">>> - 'Right': Game restart\n");
        printf(">>> Dino can get special skill if you help it reach high scores!\n");
        printf(">>> - 'Quick Drop(1000 scores)': Dino can obtain the ability to quick drop from "
               "the air\n");
        printf(">>> - 'Double Jump(2500 scores)': Dino can obtain the ability to make another jump "
               "in the air\n\n");
    }
};

} // namespace dino

#endif