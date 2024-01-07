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
    const int32_t width{35};
    const int32_t height{32};
    const int32_t bonding_offset{5};
};

static const uint8_t *bird_state_map[2] = {image_data_bird1, image_data_bird2};

class Bird
{
public:
    bool update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, bool dino_alive)
    {
        if (dino_alive) {
            if (render_cfg.last_ts - bird_tick_ > std::max(
                    static_cast<uint16_t>(render_cfg.update_interval - render_cfg.game_speed),
                    render_cfg.minimum_interval))
            {
                bird_count_ = (bird_count_ + 1) % 2;
                bird_tick_ = render_cfg.last_ts;
            }
            pos_.x -= render_cfg.game_speed / 4;
            if (pos_.x < 0 - bird_size_.width) {
                pos_.x = render_cfg.screen_width;
                is_finished_ = true;
            }
            pos_.y = render_cfg.getMiddlePaddingY() + render_cfg.getMiddlePaddingHeight() * 0.42;
        }

        screen->pushGrayscaleImage(pos_.x, pos_.y, bird_size_.width, bird_size_.height,
                                   bird_state_map[bird_count_], render_cfg.depth,
                                   render_cfg.prev_background_color, render_cfg.background_color);
        updateBoundingBox(screen);

        if (is_finished_) {
            is_finished_ = false;
            return true;
        }
        return false;
    }

    BoundingBox_t getBoundingBox() { return bounding_box_; }

private:
    void updateBoundingBox(lgfx::LGFX_Sprite *screen)
    {
        bounding_box_.upper_left.x = pos_.x;
        bounding_box_.upper_left.y = pos_.y;
        bounding_box_.lower_right.x
            = bounding_box_.upper_left.x + bird_size_.width - bird_size_.bonding_offset;
        bounding_box_.lower_right.y = bounding_box_.upper_left.y + bird_size_.height;
        // TODO(HangX-Ma): debug usage, draw box
        screen->drawRect(bounding_box_.upper_left.x, bounding_box_.upper_left.y,
                         bird_size_.width - bird_size_.bonding_offset, bird_size_.height,
                         lgfx::colors::TFT_RED);
    }

private:
    BoundingBox_t bounding_box_;
    BirdSize bird_size_;
    Position_t pos_{-0xFFFF, 0};
    uint32_t bird_count_{0};
    uint32_t bird_tick_;
    bool is_finished_{false};
};
} // namespace dino

#endif