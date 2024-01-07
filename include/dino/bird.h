#ifndef DINO_BIRD_H
#define DINO_BIRD_H

#include "dino/utils.h"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "lgfx/v1/misc/enum.hpp"

#include "assets/bird1.h"
#include "assets/bird2.h"

namespace dino
{

struct BirdSize
{
    const int32_t width{88};
    const int32_t height{80};
};

static const uint8_t *bird_state_map[6] = {image_data_bird1, image_data_bird2};

class Bird
{
public:
    bool update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg)
    {
        if (render_cfg.last_ts - bird_tick_
            > std::max(static_cast<uint16_t>(render_cfg.update_interval - render_cfg.game_speed),
                       render_cfg.minimum_interval))
        {
            bird_count_ = (bird_count_ + 1) % 2;
            bird_tick_ = render_cfg.last_ts;
        }
        pos_.x -= render_cfg.game_speed / 4;
        if (pos_.x < 0 - bird_size_.width) {
            pos_.x = render_cfg.screen_width / render_cfg.zoom_x;
            is_finished_ = true;
        }
        pos_.y = render_cfg.getBottomPaddingY() - render_cfg.getMiddlePaddingHeight() * 0.1;

        screen->pushGrayscaleImageRotateZoom(
            pos_.x, pos_.y, pos_.x, pos_.y, 0, render_cfg.zoom_x, render_cfg.zoom_y,
            bird_size_.width, bird_size_.height, bird_state_map[bird_count_], render_cfg.depth,
            render_cfg.prev_background_color, render_cfg.background_color);

        if (is_finished_) {
            is_finished_ = false;
            return true;
        }
        return false;
    }

private:
    BirdSize bird_size_;
    Position_t pos_{-0xFFFF, 0};
    uint32_t bird_count_{0};
    uint32_t bird_tick_;

    bool is_finished_{false};
};
} // namespace dino

#endif