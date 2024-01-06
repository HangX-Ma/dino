#include "dino/lv_anim.h"
#include "dino/utils.h"
#include "dino/state.h"
#include "lgfx/v1/misc/enum.hpp"

#define PIN_LEFT 39
#define PIN_DOWN 38
#define PIN_RIGHT 37
#define PIN_UP 36

namespace dino
{

GameState::~GameState()
{
    delete screen_;
    delete lcd_;
}

void GameState::setup()
{
    lcd_ = new LGFX(240, 240);
    lcd_->init();
    screen_ = new LGFX_Sprite(lcd_);
    screen_->createSprite(lcd_->width(), lcd_->height());

    // init configuration
    game_speed_ = 16;
    render_cfg_.screen_width = lcd_->width();
    render_cfg_.screen_height = lcd_->height();
    render_cfg_.last_ts = 0;
    render_cfg_.padding_ratio = 0.2;
    render_cfg_.background_color = lgfx::colors::TFT_WHITE;
    render_cfg_.prev_background_color = lgfx::colors::TFT_BLACK;
}

void GameState::loop()
{
    if (Utils::getTimestamp() - render_cfg_.last_ts > 100) {
        clearCanvas();
        renderBackground();
        renderGround();
        renderClouds();
        renderFPS();
        renderDino();
        updateCanvas();
        render_cfg_.last_ts = Utils::getTimestamp();
    }
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
    if (Utils::getTimestamp() - bg_tick_ > 5000) {
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

void GameState::renderDino() { dino_.update(screen_, render_cfg_); }
void GameState::renderGround() { ground_.update(screen_, render_cfg_, game_speed_); }
void GameState::renderClouds() { clouds_.update(screen_, render_cfg_, game_speed_); }

} // namespace dino