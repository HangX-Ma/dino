#ifndef DINO_CLOUDS_H
#define DINO_CLOUDS_H

#include "dino/random.h"
#include "dino/utils.h"
#include "assets/clouds.h"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "spdlog/spdlog.h"

namespace dino
{

using CloudsConfig_t = struct CloudsConfig
{
    const int32_t width{95};
    const int32_t height{35};
    const lgfx::color_depth_t depth{lgfx::v1::rgb332_1Byte};
    const double zoom_x{0.4};
    const double zoom_y{0.4};
};

class Clouds
{

public:
    void update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, int game_speed)
    {
        // generate new cloud
        if (Utils::getTimestamp() - clouds_tick_ > 8000) {
            int32_t y = random_generator_.next(
                render_cfg.getMiddlePaddingY() + render_cfg.getMiddlePaddingHeight() * 0.4,
                render_cfg.getMiddlePaddingY() + render_cfg.getMiddlePaddingHeight() * 0.7);
            clouds_pos_.emplace_back(
                Position_t{static_cast<int32_t>(render_cfg.screen_width / clouds_cfg_.zoom_x), y});
            clouds_tick_ = Utils::getTimestamp();
            spdlog::info("add cloud, id={}", clouds_pos_.size());
        }

        for (size_t i = 0; i < clouds_pos_.size(); i += 1) {
            clouds_pos_[i].x -= game_speed / 8.0;
            // remove invalid one
            if (clouds_pos_[i].x < 0 - clouds_cfg_.width) {
                auto cloud_iter = clouds_pos_.begin() + i;
                clouds_pos_.erase(cloud_iter);
                spdlog::info("erase cloud, id={}", i);
            }

            screen->pushGrayscaleImageRotateZoom(
                clouds_pos_[i].x, clouds_pos_[i].y, clouds_pos_[i].x, clouds_pos_[i].y, 0,
                clouds_cfg_.zoom_x, clouds_cfg_.zoom_y, clouds_cfg_.width, clouds_cfg_.height,
                image_data_clouds, clouds_cfg_.depth, render_cfg.prev_background_color,
                render_cfg.background_color);
        }
    }

private:
    Random random_generator_;
    CloudsConfig_t clouds_cfg_;
    std::vector<Position_t> clouds_pos_;
    uint32_t clouds_tick_;
};
} // namespace dino

#endif