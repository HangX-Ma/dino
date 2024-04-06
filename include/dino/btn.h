#ifndef DINO_BTN_H
#define DINO_BTN_H

#include <memory>
#include "dino/common.h"
#include "assets/btn.h"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "lgfx/v1/misc/enum.hpp"

namespace dino
{

class Btn
{
 public:
    void update(std::shared_ptr<lgfx::LGFX_Sprite> &screen)
    {
        // draw "Game Over"
        auto render_cfg = RenderConfig::getInstance();
        screen->setFont(&lgfx::fonts::Font8x8C64);
        screen->setTextDatum(lgfx::middle_center);
        screen->setTextPadding(2);
        screen->setTextSize(1.5);
        screen->setTextColor(render_cfg->prev_background_color);
        screen->drawString("Game Over", render_cfg->screen_width / 2,
                           render_cfg->getMiddlePaddingY()
                               + render_cfg->getMiddlePaddingHeight() / 3.5);
        // draw button
        screen->pushGrayscaleImage(render_cfg->screen_width / 2 - width_ / 2,
                                   render_cfg->screen_height / 2 - height_ / 2, width_, height_,
                                   image_data_btn, render_cfg->depth,
                                   render_cfg->prev_background_color, render_cfg->background_color);
    }

 private:
    int32_t width_{36};
    int32_t height_{32};
};

}  // namespace dino

#endif
