
#ifndef INCLUDE_DINO_COMMON_H_
#define INCLUDE_DINO_COMMON_H_

#include <memory>
#include <mutex>
#include <string>
#include <algorithm>
#include "lgfx/v1/misc/enum.hpp"

namespace dino
{

enum class Action
{
    NOTHING,
    JUMP,
    BEND_OVER,
    RESTART,
};
const std::string DINO_ACTION_STR[] = {"NOTHING", "JUMP", "BEND_OVER", "RESTART"};

struct RenderConfig
{
 public:
    bool dino_alive;

    int32_t screen_width;
    int32_t screen_height;
    double padding_ratio;  // range(0, 1.0)
    int32_t last_ts;       // timestamp

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
    lgfx::color_depth_t depth;

 public:
    uint16_t getUpdateInterval()
    {
        return std::max(static_cast<uint16_t>(update_interval - game_speed), minimum_interval);
    }

 public:
    int32_t getPaddingHeight() { return screen_height * padding_ratio; }
    int32_t getMiddlePaddingY() { return screen_height * padding_ratio; }
    int32_t getMiddlePaddingHeight() { return screen_height * (1.0 - 2.0 * padding_ratio); }
    int32_t getBottomPaddingY() { return screen_height * (1.0 - padding_ratio); }

 public:
    // The following configs are used for 'cloud' and 'ground' that don't need
    // do collision detection. If using the preprocessed image, the display
    // quality of 'cloud' and 'ground' will be bad!
    double zoom_x;
    double zoom_y;
    // Y is related with zoom_y, if you change zoom_y, you need to change
    // `getGroundY`
    int32_t getGroundY() { return getBottomPaddingY() + getMiddlePaddingHeight() * 0.2; }

 public:
    RenderConfig(const RenderConfig &) = delete;
    RenderConfig &operator=(const RenderConfig &) = delete;

    static std::shared_ptr<RenderConfig> getInstance()
    {
        if (singleton_ == nullptr) {
            std::unique_lock<std::mutex> lock(mt_);
            if (singleton_ == nullptr) {
                singleton_ = std::shared_ptr<RenderConfig>(new RenderConfig);
            }
        }
        return singleton_;
    }

 private:
    RenderConfig() = default;

 private:
    static std::shared_ptr<RenderConfig> singleton_;
    static std::mutex mt_;
};


}  // namespace dino

#endif  // INCLUDE_DINO_COMMON_H_
