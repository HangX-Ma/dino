#ifndef DINO_STATE_H
#define DINO_STATE_H

#include "dino/dino.h"
#include "dino/lv_anim.h"
#include "dino/random.h"
#include "dino/utils.h"
#include "dino/fps.h"

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

namespace dino
{

class GameState
{
public:
    GameState() = default;
    ~GameState();

    void setup();
    void loop();

private:
    FPS fps_;
    Dino dino_;

private:
    void switchBackground();
    void renderBackground();
    void renderMainScreen(int color);

    void generateTransitionZone(int32_t height, int32_t trans_y, double ratio);
    void renderFPS();
    void renderDino();

    void clearCanvas() { screen_->clear(); }
    void updateCanvas() { screen_->pushSprite(0, 0); }

    LGFX *lcd_{nullptr};
    LGFX_Sprite *screen_{nullptr};
    Random random_generator_;
    RenderConfig_t render_cfg_;

    // background
    bool night_mode_{false};
    lvgl::LVAnim bg_anim_;
    uint32_t bg_anim_duration_{1000};
    uint32_t bg_tick_;
};

} // namespace dino

#endif