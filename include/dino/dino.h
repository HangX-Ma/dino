#ifndef DINO_DINO_H
#define DINO_DINO_H

#include "assets/motion1.h"
#include "assets/motion2.h"
#include "assets/motion3.h"
#include "assets/motion4.h"
#include "assets/motion5.h"
#include "assets/motion6.h"

#include "dino/lv_anim.h"
#include "dino/utils.h"
#include "lgfx/v1/misc/enum.hpp"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "spdlog/spdlog.h"

#include <type_traits>

namespace dino
{

enum class DinoState : uint8_t
{
    STAND_LEFT,
    STAND_STIFF,
    STAND_RIGHT,
    DEAD,
    BEND_OVER_LEFT,
    BEND_OVER_RIGHT,
};
constexpr uint8_t STAND_STATE_NUM = 3;
constexpr uint8_t BEND_STATE_NUM = 2;

enum class DinoStatus
{
    IN_AIR,
    ON_GROUND,
    BEND_OVER,
};

const std::string DINO_STATUS_STR[3] = {
    "IN_AIR",
    "ON_GROUND",
    "BEND_OVER",
};

static const uint8_t *dino_state_map[6]
    = {image_data_motion1, image_data_motion2, image_data_motion3,
       image_data_motion4, image_data_motion5, image_data_motion6};

using DinoConfig_t = struct DinoConfig
{
    const int32_t width{88};
    const int32_t height{95};
    const lgfx::color_depth_t depth{lgfx::v1::rgb332_1Byte};
    const double zoom_x{0.4};
    const double zoom_y{0.4};
};

class Dino
{

public:
    void update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, Action action,
                bool dino_alive = true)
    {
        getGroundPos(pos_, render_cfg);

        if (!dino_alive) {
            screen->pushGrayscaleImageRotateZoom(
                pos_.x, pos_.y, pos_.x, pos_.y, 0, dino_cfg_.zoom_x, dino_cfg_.zoom_y,
                dino_cfg_.width, dino_cfg_.height, dino_state_map[Utils::toIndex(DinoState::DEAD)],
                dino_cfg_.depth, render_cfg.prev_background_color, render_cfg.background_color);
            return;
        }

        switch (status_) {
        // If Dino is on the ground, it can jump or bend over
        case DinoStatus::ON_GROUND:
        {
            switch (action) {
            case Action::BEND_OVER:
                status_ = DinoStatus::BEND_OVER;
                break;
            case Action::JUMP:
                status_ = DinoStatus::IN_AIR;
                jump(render_cfg);
                break;
            default:
                walk(screen, render_cfg, pos_);
            }
            break;
        }
        // Dino can do nothing if it jumped and still in the air
        case DinoStatus::IN_AIR:
        {
            if (stiff(screen, render_cfg, pos_)) {
                status_ = DinoStatus::ON_GROUND;
            }
            break;
        }
        // Dino bend over
        case DinoStatus::BEND_OVER:
        {
            bendOver(screen, render_cfg, pos_);
            switch (action) {
            case Action::BEND_OVER:
                status_ = DinoStatus::BEND_OVER;
                break;
            default:
                status_ = DinoStatus::ON_GROUND;
            }
            break;
        }
        }
        spdlog::info("DinoAction: {}, DinoState: {}", DINO_ACTION_STR[Utils::toIndex(action)],
                     DINO_STATUS_STR[Utils::toIndex(status_)]);
    }

private:
    /** @retval true, on ground; false, in the air */
    bool stiff(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, Position_t &pos)
    {
        // update dino 'y'
        pos_.y = dino_anim_.getValue(render_cfg.last_ts);
        screen->pushGrayscaleImageRotateZoom(
            pos.x, pos.y, pos.x, pos.y, 0, dino_cfg_.zoom_x, dino_cfg_.zoom_y, dino_cfg_.width,
            dino_cfg_.height, dino_state_map[Utils::toIndex(DinoState::STAND_STIFF)],
            dino_cfg_.depth, render_cfg.prev_background_color, render_cfg.background_color);

        // start fall down animation after reaching the top
        if (!is_falling_ && dino_anim_.isFinished(render_cfg.last_ts)) {
            dino_anim_.setPathCallback(lvgl::LVAnimPathType::EASE_IN);
            dino_anim_.setDuration(dino_anim_duration_);
            dino_anim_.setValues(pos_.y, render_cfg.getBottomPaddingY()
                                             + render_cfg.getMiddlePaddingHeight() * 0.2);
            dino_anim_.setCurrentValue(render_cfg.last_ts);
            is_falling_ = true;
            return false;
        }

        if (is_falling_ && dino_anim_.isFinished(render_cfg.last_ts)) {
            is_falling_ = false;
            return true;
        }

        return false;
    }

    void jump(RenderConfig_t &render_cfg)
    {
        dino_anim_.setPathCallback(lvgl::LVAnimPathType::EASE_OUT);
        dino_anim_.setDuration(dino_anim_duration_);
        dino_anim_.setValues(
            render_cfg.getBottomPaddingY() + render_cfg.getMiddlePaddingHeight() * 0.2,
            render_cfg.getBottomPaddingY() - render_cfg.getMiddlePaddingHeight() * 0.5);
        dino_anim_.setCurrentValue(render_cfg.last_ts);
    }

    void bendOver(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, Position_t pos)
    {
        if (render_cfg.last_ts - dino_tick_ > render_cfg.update_interval / render_cfg.game_speed) {
            bend_count_ = (bend_count_ + 1) % BEND_STATE_NUM;
            dino_tick_ = render_cfg.last_ts;
        }
        screen->pushGrayscaleImageRotateZoom(
            pos.x, pos.y, pos.x, pos.y, 0, dino_cfg_.zoom_x, dino_cfg_.zoom_y, dino_cfg_.width,
            dino_cfg_.height, dino_state_map[bend_count_ + 4], dino_cfg_.depth,
            render_cfg.prev_background_color, render_cfg.background_color);
    }

    void walk(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, Position_t pos)
    {
        if (render_cfg.last_ts - dino_tick_ > render_cfg.update_interval / render_cfg.game_speed) {
            walk_count_ = (walk_count_ + 1) % STAND_STATE_NUM;
            dino_tick_ = render_cfg.last_ts;
        }
        screen->pushGrayscaleImageRotateZoom(
            pos.x, pos.y, pos.x, pos.y, 0, dino_cfg_.zoom_x, dino_cfg_.zoom_y, dino_cfg_.width,
            dino_cfg_.height, dino_state_map[walk_count_], dino_cfg_.depth,
            render_cfg.prev_background_color, render_cfg.background_color);
    }

    void getGroundPos(Position_t &pos, RenderConfig_t &render_cfg)
    {
        pos_.x = render_cfg.screen_width * 0.2;
        pos_.y = render_cfg.getBottomPaddingY() + render_cfg.getMiddlePaddingHeight() * 0.2;
    }

private:
    uint8_t walk_count_{0};
    uint8_t bend_count_{0};

    DinoStatus status_{DinoStatus::ON_GROUND};
    DinoConfig_t dino_cfg_;
    Position_t pos_;

    bool is_falling_{false};
    uint32_t dino_tick_{0};

    lvgl::LVAnim dino_anim_;
    uint32_t dino_anim_duration_{200};
};
} // namespace dino

#endif