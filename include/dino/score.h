#ifndef DINO_SCORE_H
#define DINO_SCORE_H

#include "dino/utils.h"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "spdlog/spdlog.h"
namespace dino
{

class Score
{
public:
    void update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, bool dino_alive)
    {
        screen->setFont(&lgfx::fonts::Font8x8C64);
        screen->setTextDatum(lgfx::middle_left);
        screen->setTextPadding(2);
        screen->setTextSize(1);
        screen->setTextColor(render_cfg.prev_background_color);

        if (dino_alive) {
            if (render_cfg.last_ts - score_tick_
                > render_cfg.update_interval / render_cfg.game_speed)
            {
                score_ += 1;
                // speed up game!
                if (score_ - score_marker_ > render_cfg.score_diff) {
                    score_marker_ = score_;
                    render_cfg.game_speed
                        = std::min(static_cast<uint16_t>(render_cfg.game_speed + 1),
                                   render_cfg.max_game_speed);
                    spdlog::info("Game Speed: {}", render_cfg.game_speed);
                }
                score_tick_ = render_cfg.last_ts;
            }
        }

        score_text_ = std::to_string(score_);
        highest_score_text_ = std::to_string(highest_score_);
        uint32_t score_x = render_cfg.screen_width * 0.2;
        uint32_t highest_score_x = score_x + render_cfg.screen_width * 0.15;
        uint32_t hi_x = highest_score_x + render_cfg.screen_width * 0.1;
        uint32_t y = render_cfg.screen_height * (render_cfg.padding_ratio + 0.05);
        screen->drawString(score_text_.c_str(), render_cfg.screen_width - score_x, y);
        screen->drawString(highest_score_text_.c_str(), render_cfg.screen_width - highest_score_x,
                           y);
        screen->drawString("HI ", render_cfg.screen_width - hi_x, y);
    }

    uint32_t getScore() { return score_; }

private:
    uint32_t score_{0};
    uint32_t score_marker_{0};
    uint32_t highest_score_{0};

    std::string score_text_;
    std::string highest_score_text_;

    uint32_t score_tick_{0};
};

}; // namespace dino

#endif