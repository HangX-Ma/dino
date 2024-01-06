#ifndef DINO_GROUND_H
#define DINO_GROUND_H

#include "dino/utils.h"
#include "assets/ground.h"
#include "lgfx/v1/LGFX_Sprite.hpp"

namespace dino
{

using GroundConfig_t = struct GroundConfig
{
    const int32_t width{2400};
    const int32_t height{20};
    const lgfx::color_depth_t depth{lgfx::v1::rgb332_1Byte};
    const double zoom_x{0.4};
    const double zoom_y{0.4};
};

class Ground
{
public:
    void update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg)
    {
        if (render_cfg.last_ts - ground_tick_ > render_cfg.update_interval / render_cfg.game_speed)
        {
            pos_.x -= render_cfg.game_speed;
            if (pos_.x < -(ground_cfg_.width * ground_cfg_.zoom_x - render_cfg.screen_width)) {
                pos_.x = 0;
            }
            ground_tick_ = render_cfg.last_ts;
        }
        pos_.y = render_cfg.getBottomPaddingY() + render_cfg.getMiddlePaddingHeight() * 0.2 + 64;
        screen->pushGrayscaleImageRotateZoom(
            pos_.x, pos_.y, pos_.x, pos_.y, 0, ground_cfg_.zoom_x, ground_cfg_.zoom_y,
            ground_cfg_.width, ground_cfg_.height, image_data_ground, ground_cfg_.depth,
            render_cfg.prev_background_color, render_cfg.background_color);
    }

private:
    GroundConfig_t ground_cfg_;
    Position_t pos_{0, 0};
    uint32_t ground_tick_{0};
};

} // namespace dino

#endif