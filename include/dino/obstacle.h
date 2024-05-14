
#ifndef INCLUDE_DINO_OBSTACLE_H_
#define INCLUDE_DINO_OBSTACLE_H_

#include <memory>
#include "dino/utils.h"
#include "dino/common.h"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "spdlog/spdlog.h"

namespace dino
{

using ObstacleSize_t = struct ObstacleSize
{
    int32_t width;
    int32_t height;
    int32_t bonding_offset;
};

class Obstacle
{
 public:
    Obstacle(int width, int height, int offset = 0)
        : Obstacle(ObstacleSize_t{width, height, offset})
    {
    }

    explicit Obstacle(const ObstacleSize_t &size) : size(size) {}
    virtual ~Obstacle() = default;

    virtual void resetState()
    {
        tick = 0;
        tick_updated = false;
        is_finished = false;
    }

     bool update(std::shared_ptr<lgfx::LGFX_Sprite> &screen, Position_t &position,
                const Position_t &new_position, const uint8_t *image)
    {
        // spdlog::info("prev position: ({},{}), new position: ({},{})", position.x, position.y, position.x - new_position.x, new_position.y);
        auto render_cfg = RenderConfig::getInstance();
        if (render_cfg->dino_alive) {
            // calculate current canvas update rate
            auto &&update_interval = render_cfg->getUpdateInterval();
            if (render_cfg->last_ts - tick > update_interval) {
                tick_updated = true;
                tick = render_cfg->last_ts;
                // move cactus
                position.x -= new_position.x;
                if (position.x < 0 - size.width) {
                    position.x = render_cfg->screen_width;
                    is_finished = true;
                }
            }
            position.y = new_position.y;
        }

        screen->pushGrayscaleImage(position.x, position.y, size.width, size.height, image,
                                   render_cfg->depth, render_cfg->prev_background_color,
                                   render_cfg->background_color);
        updateBoundingBox(position);

        // TODO(HangX-Ma): debug usage, draw box
        // screen->drawRect(bounding_box.upper_left.x, bounding_box.upper_left.y, size.width,
        //                  size.height, lgfx::colors::TFT_RED);

        if (is_finished) {
            is_finished = false;
            return true;
        }
        return false;
    }

    // This function will return true if tick updates and automatically reset flag.
    // It will not influence original false state.
    bool checkTickUpdate()
    {
        bool retval = tick_updated;
        tick_updated = false;
        return retval;
    }
    virtual const BoundingBox &getBoundingBox() const { return bounding_box; }

 private:
    void updateBoundingBox(const Position_t &position)
    {
        bounding_box.upper_left.x = position.x;
        bounding_box.upper_left.y = position.y;
        bounding_box.lower_right.x = bounding_box.upper_left.x + size.width;
        bounding_box.lower_right.y = bounding_box.upper_left.y + size.height;
    }

 protected:
    // animation property
    uint32_t tick{0};
    bool tick_updated{false};
    bool is_finished{false};
    // obstacle property
    ObstacleSize_t size;
    BoundingBox_t bounding_box;
};

}  // namespace dino

#endif  // INCLUDE_DINO_OBSTACLE_H_
