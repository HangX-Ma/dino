#ifndef DINO_BTN_H
#define DINO_BTN_H

#include "dino/utils.h"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "assets/btn.h"
#include "lgfx/v1/misc/enum.hpp"

namespace dino
{

struct BtnSize
{
    const int32_t width{36};
    const int32_t height{32};
};

class Btn
{
public:
    void update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg)
    {
        // draw "Game Over"
        screen->setFont(&lgfx::fonts::Font8x8C64);
        screen->setTextDatum(lgfx::middle_center);
        screen->setTextPadding(2);
        screen->setTextSize(1.5);
        screen->setTextColor(render_cfg.prev_background_color);
        screen->drawString("Game Over", render_cfg.screen_width / 2,
                           render_cfg.getMiddlePaddingY()
                               + render_cfg.getMiddlePaddingHeight() / 3.5);
        // draw button
        screen->pushGrayscaleImage(render_cfg.screen_width / 2 - btn_size_.width / 2,
                                   render_cfg.screen_height / 2 - btn_size_.height / 2,
                                   btn_size_.width, btn_size_.height, image_data_btn,
                                   render_cfg.depth, render_cfg.prev_background_color,
                                   render_cfg.background_color);
    }

private:
    BtnSize btn_size_;
};

} // namespace dino

#endif