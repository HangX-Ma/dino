#ifndef DINO_CACTUS_H
#define DINO_CACTUS_H

#include "assets/cactus1.h"
#include "assets/cactus2.h"
#include "assets/cactus3.h"
#include "dino/lv_anim.h"
#include "dino/utils.h"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "lgfx/v1/misc/enum.hpp"

namespace dino
{

struct CactusSize
{
    int32_t width;
    int32_t height;
};

using CactusConfig_t = struct CactusConfig
{
    CactusSize cactus_size[3];
};

enum class CactusType : uint8_t
{
    LARGE = 0,
    MIDDLE,
    SMALL,
};

static const uint8_t *cactus_type_map[3]
    = {image_data_cactus1, image_data_cactus2, image_data_cactus3};

class Cactus
{
public:
    Cactus()
    {
        auto &large = cactus_cfg_.cactus_size[Utils::toIndex(CactusType::LARGE)];
        large.width = 51;
        large.height = 32;

        auto &middle = cactus_cfg_.cactus_size[Utils::toIndex(CactusType::MIDDLE)];
        middle.width = 20;
        middle.height = 32;

        auto &small = cactus_cfg_.cactus_size[Utils::toIndex(CactusType::SMALL)];
        small.width = 16;
        small.height = 32;
    }

    bool update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, CactusType type,
                bool dino_alive)
    {
        if (dino_alive) {
            if (render_cfg.last_ts - cactus_tick_ > std::max(
                    static_cast<uint16_t>(render_cfg.update_interval - render_cfg.game_speed),
                    render_cfg.minimum_interval))
            {
                cactus_tick_ = render_cfg.last_ts;
                pos_.x -= render_cfg.game_speed;
                if (pos_.x < 0 - cactus_cfg_.cactus_size[Utils::toIndex(type)].width) {
                    pos_.x = render_cfg.screen_width;
                    is_finished_ = true;
                }
            }
            pos_.y = render_cfg.getBottomPaddingY() - render_cfg.getMiddlePaddingHeight() * 0.3;
        }

        screen->pushGrayscaleImage(pos_.x, pos_.y,
                                   cactus_cfg_.cactus_size[Utils::toIndex(type)].width,
                                   cactus_cfg_.cactus_size[Utils::toIndex(type)].height,
                                   cactus_type_map[Utils::toIndex(type)], render_cfg.depth,
                                   render_cfg.prev_background_color, render_cfg.background_color);
        updateBoundingBox(screen, type);

        if (is_finished_) {
            is_finished_ = false;
            return true;
        }
        return false;
    }

    void reset()
    {
        pos_ = {-0xFFFF, 0};
        cactus_tick_ = 0;
        is_finished_ = false;
    }

    BoundingBox_t getBoundingBox() { return bounding_box_; }

private:
    void updateBoundingBox(lgfx::LGFX_Sprite *screen, CactusType type)
    {
        bounding_box_.upper_left.x = pos_.x;
        bounding_box_.upper_left.y = pos_.y;
        bounding_box_.lower_right.x
            = bounding_box_.upper_left.x + cactus_cfg_.cactus_size[Utils::toIndex(type)].width;
        bounding_box_.lower_right.y
            = bounding_box_.upper_left.y + cactus_cfg_.cactus_size[Utils::toIndex(type)].height;
        // TODO(HangX-Ma): debug usage, draw box
        // screen->drawRect(bounding_box_.upper_left.x, bounding_box_.upper_left.y,
        //                  cactus_cfg_.cactus_size[Utils::toIndex(type)].width,
        //                  cactus_cfg_.cactus_size[Utils::toIndex(type)].height,
        //                  lgfx::colors::TFT_RED);
    }

private:
    BoundingBox_t bounding_box_;
    CactusConfig_t cactus_cfg_;
    Position_t pos_{-0xFFFF, 0};
    uint32_t cactus_tick_{0};
    bool is_finished_{false};
};
} // namespace dino

#endif