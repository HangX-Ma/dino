#ifndef INCLUDE_DINO_SCORE_H_
#define INCLUDE_DINO_SCORE_H_

#include <algorithm>
#include <memory>
#include <string>
#include "dino/common.h"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "spdlog/spdlog.h"
namespace dino
{

class Score
{
 public:
    void update(std::shared_ptr<lgfx::LGFX_Sprite> &screen)
    {
        auto render_cfg = RenderConfig::getInstance();
        screen->setFont(&lgfx::fonts::Font8x8C64);
        screen->setTextDatum(lgfx::middle_left);
        screen->setTextPadding(2);
        screen->setTextSize(1);
        screen->setTextColor(render_cfg->prev_background_color);

        if (render_cfg->dino_alive) {
            if (render_cfg->last_ts - score_tick_ > render_cfg->getUpdateInterval()) {
                score_ += 1;
                // speed up game!
                if (score_ - score_marker_ > render_cfg->score_diff) {
                    score_marker_ = score_;
                    marker_count_ += 1;
                    render_cfg->game_speed
                        = std::min(static_cast<uint16_t>(render_cfg->game_speed + 1),
                                   render_cfg->max_game_speed);
                    spdlog::info("Another 500 point! Game speed increase, now {}",
                                 render_cfg->game_speed);
                }
                score_tick_ = render_cfg->last_ts;
            }
        }

        score_text_ = std::to_string(score_);
        highest_score_text_ = std::to_string(highest_score_);
        uint32_t score_x = render_cfg->screen_width * 0.2;
        uint32_t highest_score_x = score_x + render_cfg->screen_width * 0.15;
        uint32_t hi_x = highest_score_x + render_cfg->screen_width * 0.1;
        uint32_t y = render_cfg->screen_height * (render_cfg->padding_ratio + 0.05);
        screen->drawString(score_text_.c_str(), render_cfg->screen_width - score_x, y);
        screen->drawString(highest_score_text_.c_str(), render_cfg->screen_width - highest_score_x,
                           y);
        screen->drawString("HI ", render_cfg->screen_width - hi_x, y);
    }

    void reset()
    {
        if (highest_score_ < score_) {
            highest_score_ = score_;
        }
        score_ = 0;
        score_marker_ = 0;
        score_tick_ = 0;
        marker_count_ = 0;
    }

    uint32_t getScore() { return score_; }
    uint32_t getMarkerCount() { return marker_count_; }
    void clearMarkerCount() { marker_count_ = 0; }

 private:
    uint32_t score_{0};
    uint32_t score_marker_{0};
    uint32_t highest_score_{0};
    uint32_t marker_count_{0};

    std::string score_text_;
    std::string highest_score_text_;

    uint32_t score_tick_{0};
};

};  // namespace dino

#endif  // INCLUDE_DINO_SCORE_H_
