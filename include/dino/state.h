#ifndef DINO_STATE_H
#define DINO_STATE_H

#include <memory>

#include "dino/btn.h"
#include "dino/clouds.h"
#include "dino/dino.h"
#include "dino/ground.h"
#include "dino/lv_anim.h"
#include "dino/obstacle_manager.h"
#include "dino/random.h"
#include "dino/score.h"
#include "dino/fps.h"

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

namespace dino
{

class GameState
{
 public:
    void setup();
    void loop();

 private:
    FPS fps_;
    Dino dino_;
    Ground ground_;
    Clouds clouds_;
    Score score_;
    ObstacleManager obstacle_;
    Btn btn_;

 private:
    void switchBackground();
    void renderBackground();
    void renderMainScreen(int color);

    void generateTransitionZone(int32_t height, int32_t trans_y, double ratio);
    void renderFPS();
    void renderDino();
    void renderGround();
    void renderClouds();
    void renderScore();
    void renderObstacle();
    void renderBtn();

    void renderDialogOpen();
    void renderDialogClose();
    void renderQuickDropSkillDialog();
    void renderDoubleJumpSkillDialog();

    void clearCanvas() { screen_->clear(); }
    void updateCanvas() { screen_->pushSprite(0, 0); }
    void scanKeyboard();

    void reset();

 private:
    std::shared_ptr<LGFX> lcd_{nullptr};
    std::shared_ptr<LGFX_Sprite> screen_{nullptr};
    Random random_generator_;

 private:
    // background
    bool night_mode_{false};
    lvgl::LVAnim bg_anim_;
    uint32_t bg_anim_duration_{1000};
    uint32_t bg_tick_;
    // keyboard
    Action action_{Action::NOTHING};
};

}  // namespace dino

#endif
