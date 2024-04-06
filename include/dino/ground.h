#ifndef INCLUDE_DINO_GROUND_H_
#define INCLUDE_DINO_GROUND_H_

#include <memory>
#include "dino/utils.h"
#include "dino/common.h"
#include "assets/ground.h"
#include "lgfx/v1/LGFX_Sprite.hpp"

namespace dino
{

struct GroundSize
{
    const int32_t width{2400};
    const int32_t height{20};
};

class Ground
{
 public:
    void update(std::shared_ptr<lgfx::LGFX_Sprite> &screen)
    {
        auto render_cfg = RenderConfig::getInstance();
        if (render_cfg->dino_alive) {
            auto &&update_interval = render_cfg->getUpdateInterval();
            if (render_cfg->last_ts - ground_tick_ > update_interval) {
                position_.x -= render_cfg->game_speed;
                if (position_.x < -(width_ * render_cfg->zoom_x - render_cfg->screen_width)) {
                    position_.x = 0;
                }
                ground_tick_ = render_cfg->last_ts;
            }
            position_.y
                = render_cfg->getBottomPaddingY() + render_cfg->getMiddlePaddingHeight() * 0.2 + 64;
        }

        screen->pushGrayscaleImageRotateZoom(
            position_.x, position_.y, position_.x, position_.y, 0, render_cfg->zoom_x,
            render_cfg->zoom_y, width_, height_, image_data_ground, render_cfg->depth,
            render_cfg->prev_background_color, render_cfg->background_color);
    }

 private:
    int32_t width_{2400};
    int32_t height_{20};
    Position_t position_{0, 0};
    uint32_t ground_tick_{0};
};

}  // namespace dino

#endif  // INCLUDE_DINO_GROUND_H_
