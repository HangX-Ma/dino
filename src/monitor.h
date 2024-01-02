#pragma once
#include "lgfx/v1/platforms/sdl/common.hpp"
#include "lgfx/v1_autodetect/LGFX_AutoDetect_sdl.hpp"

#include <numeric>

class Monitor
{
public:
    Monitor()
    {
        display_ = new LGFX(180, 60);
        display_->init();
        canvas_ = new LGFX_Sprite(display_);
        canvas_->createSprite(display_->width(), display_->height());
        canvas_->setFont(&fonts::efontCN_24);

        time_ticks_ = lgfx::micros();
    }

    ~Monitor()
    {
        delete canvas_;
        delete display_;
    }

    void update()
    {
        delta_ticks_ = lgfx::micros() - last_ticks_;
        avg_vec_.push_back(delta_ticks_);
        if (lgfx::micros() - time_ticks_ >= 500000) {
            double avg = static_cast<double>(std::accumulate(avg_vec_.begin(), avg_vec_.end(), 0LU))
                         / avg_vec_.size();
            canvas_->fillScreen(TFT_BLACK);
            canvas_->setCursor(0, 0);
            canvas_->printf("% 10f ms\n", avg / 1000.0);
            canvas_->printf("% 10ld fps", static_cast<uint64_t>(1000000 / avg));
            canvas_->pushSprite(0, 0);

            avg_vec_.clear();
            time_ticks_ = lgfx::micros();
        }
        last_ticks_ = lgfx::micros();
    }

private:
    LGFX *display_;
    LGFX_Sprite *canvas_;
    uint64_t last_ticks_;
    uint64_t delta_ticks_;
    uint64_t time_ticks_;
    std::vector<uint64_t> avg_vec_;
};
