#include "dino/cactus.h"
#include "dino/dino.h"
#include "dino/lv_anim.h"
#include "dino/utils.h"
#include "dino/state.h"
#include "lgfx/v1/misc/enum.hpp"

#define BTN_DOWN 38  // bend over
#define BTN_RIGHT 37 // restart
#define BTN_UP 36    // jump

namespace dino
{

GameState::~GameState()
{
    delete screen_;
    delete lcd_;
}

void GameState::setup()
{
    Utils::showInfo();
    Utils::showUsage();

    lcd_ = new LGFX(240, 240);
    lcd_->init();
    screen_ = new LGFX_Sprite(lcd_);
    screen_->createSprite(lcd_->width(), lcd_->height());

    // init configuration
    render_cfg_.update_interval = 100;
    render_cfg_.minimum_interval = 60;
    render_cfg_.game_speed = 12;
    render_cfg_.max_game_speed = render_cfg_.update_interval - render_cfg_.minimum_interval;

    render_cfg_.bird_come_score = 1200;
    render_cfg_.score_diff = 500;
    // DONE(HangX-Ma): change back
    render_cfg_.score_skill_quick_drop = 1000;
    render_cfg_.score_skill_double_jump = 2500;
    render_cfg_.obstacle_max_delay = 2000; // 2s

    render_cfg_.screen_width = lcd_->width();
    render_cfg_.screen_height = lcd_->height();
    render_cfg_.last_ts = 0;

    render_cfg_.padding_ratio = 0.2;
    render_cfg_.background_color = lgfx::colors::TFT_WHITE;
    render_cfg_.prev_background_color = lgfx::colors::TFT_BLACK;

    render_cfg_.depth = lgfx::v1::rgb332_1Byte;
    render_cfg_.zoom_x = 0.4;
    render_cfg_.zoom_y = 0.4;
}

void GameState::loop()
{
    if (Utils::getTimestamp() - render_cfg_.last_ts > 20) {
        clearCanvas();
        renderBackground();
        renderGround();
        renderClouds();
        renderFPS();
        renderScore();
        renderDino();
        renderObstacle();
        renderBtn();
        updateCanvas();
        render_cfg_.last_ts = Utils::getTimestamp();
    }
    scanKeyboard();
}

void GameState::switchBackground()
{
    night_mode_ = !night_mode_;
    bg_anim_.setPathCallback(lvgl::LVAnimPathType::BOUNCE);
    bg_anim_.setDuration(bg_anim_duration_);
    bg_anim_.setValues(0, render_cfg_.getBottomPaddingY());
    bg_anim_.setCurrentValue(render_cfg_.last_ts);
    bg_tick_ = render_cfg_.last_ts;

    if (night_mode_) {
        render_cfg_.background_color = lgfx::colors::TFT_BLACK;
        render_cfg_.prev_background_color = lgfx::colors::TFT_WHITE;
    }
    else {
        render_cfg_.background_color = lgfx::colors::TFT_WHITE;
        render_cfg_.prev_background_color = lgfx::colors::TFT_BLACK;
    }
}

void GameState::renderBackground()
{
    // draw padding
    screen_->fillScreen(lgfx::colors::TFT_WHITE);
    screen_->fillRectAlpha(0, 0, screen_->width(), render_cfg_.getPaddingHeight(), 100, TFT_BLACK);
    screen_->fillRectAlpha(0, render_cfg_.getBottomPaddingY(), screen_->width(),
                           render_cfg_.getPaddingHeight(), 100, TFT_BLACK);

    // draw white background and gray padding
    if (score_.getMarkerCount() >= 1) {
        score_.clearMarkerCount();
        switchBackground();
    }

    if (bg_anim_.isFinished(render_cfg_.last_ts)) {
        // Don't change current background if animation done
        renderMainScreen(render_cfg_.background_color);
        return;
    }

    // Don't change the previous background color
    renderMainScreen(render_cfg_.prev_background_color);

    // Draw path animation
    int32_t target_height = bg_anim_.getValue(render_cfg_.last_ts);
    screen_->setColor(render_cfg_.background_color);
    screen_->fillRect(0, render_cfg_.getMiddlePaddingY(), screen_->width(),
                      std::min(target_height, render_cfg_.getMiddlePaddingHeight()));

    // Draw transition
    int32_t lower_trans_height
        = std::min(static_cast<int32_t>(screen_->height() * 0.1), target_height / 2);
    int32_t lower_trans_y = render_cfg_.getMiddlePaddingY() + target_height - lower_trans_height;
    generateTransitionZone(lower_trans_height, lower_trans_y,
                           random_generator_.nextDouble(0.08, 0.1));

    if (lower_trans_height < target_height / 2) {
        int32_t upper_trans_height = std::min(static_cast<int32_t>(screen_->height() * 0.12),
                                              target_height - lower_trans_height);
        int32_t upper_trans_y = render_cfg_.getMiddlePaddingY() + target_height - lower_trans_height
                                - upper_trans_height;
        generateTransitionZone(upper_trans_height, upper_trans_y,
                               random_generator_.nextDouble(0.02, 0.04));
    }
}

void GameState::renderMainScreen(int color)
{
    screen_->setColor(color);
    screen_->fillRect(0, render_cfg_.getMiddlePaddingY(), screen_->width(),
                      render_cfg_.getMiddlePaddingHeight());
}

void GameState::generateTransitionZone(int32_t height, int32_t trans_y, double ratio)
{
    int dot_nums = height * screen_->width() * ratio;
    for (int i = 0; i < dot_nums; i += 1) {
        int32_t x = random_generator_.next(0, screen_->width() - 1);
        int32_t y = std::min(random_generator_.next(trans_y, trans_y + height),
                             render_cfg_.getBottomPaddingY());
        screen_->drawPixel(x, y, render_cfg_.prev_background_color);
    }
}

void GameState::renderFPS()
{
    auto fps_text = fps_.update();
    screen_->setFont(&lgfx::fonts::Font8x8C64);
    screen_->setTextDatum(lgfx::middle_left);
    screen_->setTextPadding(2);
    screen_->setTextSize(1);
    screen_->setTextColor(render_cfg_.prev_background_color);
    screen_->drawString(
        fps_text.c_str(), static_cast<int32_t>(screen_->width() / 20.0),
        static_cast<int32_t>(screen_->height() * (render_cfg_.padding_ratio + 0.05)));
}

void GameState::renderDino()
{
    dino_.update(screen_, render_cfg_, action_);
    bool collide = Utils::intersects(dino_.getBoundingBox(), obstacle_.getBoundingBox());
    if (collide) {
        dino_.setDinoAliveStatus(false);
    }
}
void GameState::renderGround() { ground_.update(screen_, render_cfg_, dino_.getDinoAliveStatus()); }
void GameState::renderClouds() { clouds_.update(screen_, render_cfg_); }
void GameState::renderScore() { score_.update(screen_, render_cfg_, dino_.getDinoAliveStatus()); }
void GameState::renderObstacle()
{
    obstacle_.update(screen_, render_cfg_, score_.getScore(), dino_.getDinoAliveStatus());
}

void GameState::renderDialogOpen()
{
    for (int w = 0; w < screen_->width() * 0.8; w += 1) {
        screen_->fillRoundRect((screen_->width() * 0.1 + 3) + screen_->width() * 0.4 - (w / 2.0),
                               screen_->height() * 0.25 + 3, w, screen_->height() * 0.5, 8,
                               lgfx::colors::TFT_LIGHTGRAY);
        screen_->fillRoundRect((screen_->width() * 0.1) + screen_->width() * 0.4 - (w / 2.0),
                               screen_->height() * 0.25, w, screen_->height() * 0.5, 8,
                               lgfx::colors::TFT_DARKGRAY);
        SDL_Delay(2);
        updateCanvas();
    }
    SDL_Delay(50);
}

void GameState::renderDialogClose()
{
    for (int w = screen_->width() * 0.8; w > 0; w -= 1) {
        screen_->clear();
        renderBackground();
        screen_->fillRoundRect((screen_->width() * 0.1 + 3) + screen_->width() * 0.4 - (w / 2.0),
                               screen_->height() * 0.25 + 3, w, screen_->height() * 0.5, 8,
                               lgfx::colors::TFT_LIGHTGRAY);
        screen_->fillRoundRect((screen_->width() * 0.1) + screen_->width() * 0.4 - (w / 2.0),
                               screen_->height() * 0.25, w, screen_->height() * 0.5, 8,
                               lgfx::colors::TFT_DARKGRAY);
        SDL_Delay(2);
        updateCanvas();
    }
}

void GameState::renderQuickDropSkillDialog()
{
    renderDialogOpen();

    screen_->setFont(&fonts::Font8x8C64);
    screen_->setTextDatum(middle_left);
    screen_->setTextColor(TFT_WHITE, lgfx::colors::TFT_WHITE);

    screen_->setTextSize(1.2);
    screen_->drawCenterString("Congratulations!", screen_->width() / 2 - 3,
                              render_cfg_.getMiddlePaddingY()
                                  + render_cfg_.getMiddlePaddingHeight() * 0.2);

    screen_->setTextSize(1);
    screen_->drawString("Your Dino gets a new", screen_->width() / 6,
                        render_cfg_.getMiddlePaddingY()
                            + render_cfg_.getMiddlePaddingHeight() * 0.35);
    screen_->drawString("skill. You can press", screen_->width() / 6,
                        render_cfg_.getMiddlePaddingY()
                            + render_cfg_.getMiddlePaddingHeight() * 0.45);
    screen_->drawString("down arrow in air to", screen_->width() / 6,
                        render_cfg_.getMiddlePaddingY()
                            + render_cfg_.getMiddlePaddingHeight() * 0.55);
    screen_->drawString("make Dino back to the", screen_->width() / 6,
                        render_cfg_.getMiddlePaddingY()
                            + render_cfg_.getMiddlePaddingHeight() * 0.65);
    screen_->drawString("ground right away!", screen_->width() / 6,
                        render_cfg_.getMiddlePaddingY()
                            + render_cfg_.getMiddlePaddingHeight() * 0.75);
    updateCanvas();

    // wait user to confirm
    while (true) {
        scanKeyboard();
        if (action_ == Action::RESTART) {
            action_ = Action::NOTHING;
            dino_.setDinoAliveStatus(true);
            break;
        }
    }
    renderDialogClose();
}

void GameState::renderDoubleJumpSkillDialog()
{
    renderDialogOpen();

    screen_->setFont(&fonts::Font8x8C64);
    screen_->setTextDatum(middle_left);
    screen_->setTextColor(TFT_WHITE, lgfx::colors::TFT_WHITE);

    screen_->setTextSize(1.2);
    screen_->drawCenterString("Congratulations!", screen_->width() / 2 - 3,
                              render_cfg_.getMiddlePaddingY()
                                  + render_cfg_.getMiddlePaddingHeight() * 0.2);

    screen_->setTextSize(1);
    screen_->drawString("Your Dino gets anoth", screen_->width() / 6,
                        render_cfg_.getMiddlePaddingY()
                            + render_cfg_.getMiddlePaddingHeight() * 0.35);
    screen_->drawString("-er new skill. You ", screen_->width() / 6,
                        render_cfg_.getMiddlePaddingY()
                            + render_cfg_.getMiddlePaddingHeight() * 0.45);
    screen_->drawString("can press up arrow ", screen_->width() / 6,
                        render_cfg_.getMiddlePaddingY()
                            + render_cfg_.getMiddlePaddingHeight() * 0.55);
    screen_->drawString("in air to make Dino", screen_->width() / 6,
                        render_cfg_.getMiddlePaddingY()
                            + render_cfg_.getMiddlePaddingHeight() * 0.65);
    screen_->drawString("do another jump!", screen_->width() / 6,
                        render_cfg_.getMiddlePaddingY()
                            + render_cfg_.getMiddlePaddingHeight() * 0.75);
    updateCanvas();

    // wait user to confirm
    while (true) {
        scanKeyboard();
        if (action_ == Action::RESTART) {
            action_ = Action::NOTHING;
            dino_.setDinoAliveStatus(true);
            break;
        }
    }
    renderDialogClose();
}

void GameState::renderBtn()
{
    if (!dino_.getDinoAliveStatus()) {
        btn_.update(screen_, render_cfg_);
        // check restart command
        if (action_ == Action::RESTART) {
            // quick drop skill
            if (!dino_.getDinoSkillBag().type.quick_drop
                && score_.getScore() >= render_cfg_.score_skill_quick_drop)
            {
                renderQuickDropSkillDialog();
                dino_.setDinoNewSkill(DinoSkillType::QUICK_DROP);
            }
            // double jump skill
            if (!dino_.getDinoSkillBag().type.double_jump
                && score_.getScore() >= render_cfg_.score_skill_double_jump)
            {
                renderDoubleJumpSkillDialog();
                dino_.setDinoNewSkill(DinoSkillType::DOUBLE_JUMP);
            }
            reset();
        }
    }
}

void GameState::scanKeyboard()
{
    if (!lgfx::gpio_in(BTN_DOWN)) {
        action_ = Action::BEND_OVER;
    }
    else if (!lgfx::gpio_in(BTN_RIGHT)) {
        action_ = Action::RESTART;
    }
    else if (!lgfx::gpio_in(BTN_UP)) {
        action_ = Action::JUMP;
    }
    else {
        action_ = Action::NOTHING;
    }
}

void GameState::reset()
{
    dino_.reset();
    obstacle_.reset();
    clouds_.reset();
    score_.reset();
    // restore global config
    render_cfg_.game_speed = 12;
    render_cfg_.background_color = lgfx::colors::TFT_WHITE;
    render_cfg_.prev_background_color = lgfx::colors::TFT_BLACK;
}

} // namespace dino