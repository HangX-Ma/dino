#ifndef DINO_DINO_H
#define DINO_DINO_H

#include "assets/motion1.h"
#include "assets/motion2.h"
#include "assets/motion3.h"
#include "assets/motion4.h"
#include "assets/motion5.h"
#include "assets/motion6.h"

#include "dino/utils.h"
#include "lgfx/v1/misc/enum.hpp"
#include "lgfx/v1/LGFX_Sprite.hpp"

#include <type_traits>

namespace dino
{

enum class DinoState : uint8_t
{
    STAND_LEFT,
    STAND_STILL,
    STAND_RIGHT,
    DEAD,
    BEND_OVER_LEFT,
    BEND_OVER_RIGHT,
};
constexpr uint8_t STAND_STATE_NUM = 3;

static const uint8_t *dino_state_map[6]
    = {image_data_motion1, image_data_motion2, image_data_motion3,
       image_data_motion4, image_data_motion5, image_data_motion6};

using DinoConfig_t = struct DinoConfig
{
    const int32_t width{88};
    const int32_t height{95};
    const lgfx::color_depth_t depth{lgfx::v1::rgb332_1Byte};
    const double zoom_x{0.4};
    const double zoom_y{0.4};
    Position_t pos;
};

class Dino
{

public:
    void update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg)
    {
        dino_cfg_.pos.x = render_cfg.screen_width * 0.2;
        dino_cfg_.pos.y
            = render_cfg.getBottomPaddingY() + render_cfg.getMiddlePaddingHeight() * 0.2;
        walk(screen, dino_cfg_.pos, render_cfg);
    }

private:
    void walk(lgfx::LGFX_Sprite *screen, Position_t pos, RenderConfig_t &render_cfg)
    {
        walk_count_ = (walk_count_ + 1) % STAND_STATE_NUM;
        screen->pushGrayscaleImageRotateZoom(
            pos.x, pos.y, pos.x, pos.y, 0, dino_cfg_.zoom_x, dino_cfg_.zoom_y, dino_cfg_.width,
            dino_cfg_.height, dino_state_map[walk_count_], dino_cfg_.depth,
            render_cfg.prev_background_color, render_cfg.background_color);
    }

private:
    uint8_t walk_count_{0};
    DinoConfig_t dino_cfg_;
};
} // namespace dino

#endif