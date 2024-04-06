#ifndef INCLUDE_DINO_CLOUDS_H_
#define INCLUDE_DINO_CLOUDS_H_

#include <memory>
#include <vector>
#include "dino/random.h"
#include "dino/common.h"
#include "dino/utils.h"
#include "assets/clouds.h"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "lgfx/v1/misc/enum.hpp"

namespace dino
{

class Clouds
{
 public:
    void update(std::shared_ptr<lgfx::LGFX_Sprite> &screen)
    {
        // generate new cloud
        auto render_cfg = RenderConfig::getInstance();
        if (Utils::getTimestamp() - clouds_tick_ > 6000) {
            int32_t y = random_generator_.next(
                render_cfg->getMiddlePaddingY()
                    + render_cfg->getMiddlePaddingHeight() * 0.18 / render_cfg->zoom_y,
                render_cfg->getMiddlePaddingY()
                    + render_cfg->getMiddlePaddingHeight() * 0.35 / render_cfg->zoom_y);
            clouds_pos_.emplace_back(
                Position_t{static_cast<int32_t>(render_cfg->screen_width / render_cfg->zoom_x), y});
            clouds_tick_ = Utils::getTimestamp();
        }

        for (size_t i = 0; i < clouds_pos_.size(); i += 1) {
            clouds_pos_[i].x -= 1;
            // remove invalid one
            if (clouds_pos_[i].x < 0 - width_) {
                auto cloud_iter = clouds_pos_.begin() + i;
                clouds_pos_.erase(cloud_iter);
            }

            screen->pushGrayscaleImageRotateZoom(
                clouds_pos_[i].x, clouds_pos_[i].y, clouds_pos_[i].x, clouds_pos_[i].y, 0,
                render_cfg->zoom_x, render_cfg->zoom_y, width_, height_, image_data_clouds,
                render_cfg->depth, lgfx::colors::TFT_DARKGRAY, render_cfg->background_color);
        }
    }

    void reset()
    {
        clouds_pos_.clear();
        clouds_tick_ = 0;
    }

 private:
    int32_t width_{95};
    int32_t height_{35};
    Random random_generator_;
    std::vector<Position_t> clouds_pos_;
    uint32_t clouds_tick_{0};
};
}  // namespace dino

#endif  // INCLUDE_DINO_CLOUDS_H_
