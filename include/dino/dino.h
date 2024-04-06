#ifndef INCLUDE_DINO_DINO_H_
#define INCLUDE_DINO_DINO_H_

#include <memory>
#include <string>
#include "assets/motion1.h"
#include "assets/motion2.h"
#include "assets/motion3.h"
#include "assets/motion4.h"
#include "assets/motion5.h"
#include "assets/motion6.h"

#include "dino/obstacle.h"
#include "dino/lv_anim.h"
#include "dino/utils.h"
#include "lgfx/v1/LGFX_Sprite.hpp"
#include "spdlog/spdlog.h"

namespace dino
{

enum class DinoSkillType : uint8_t
{
    QUICK_DROP,
    DOUBLE_JUMP,
};

union DinoSkillBag
{
    uint8_t skill_bitwise;
    struct Skill
    {
        uint8_t quick_drop : 1;
        uint8_t double_jump : 1;
        uint8_t : 6;
    } type;
};

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

class Dino : public Obstacle
{
 public:
    explicit Dino(int width = 33, int height = 36, int offset = 12)
        : Obstacle(width, height, offset)
    {
    }

    void update(std::shared_ptr<lgfx::LGFX_Sprite> &screen, Action action)
    {
        auto render_cfg = RenderConfig::getInstance();
        if (!render_cfg->dino_alive) {
            screen->pushGrayscaleImage(position_.x, position_.y, size.width, size.height,
                                       dino_state_map[Utils::toIndex(DinoState::DEAD)],
                                       render_cfg->depth, render_cfg->prev_background_color,
                                       render_cfg->background_color);
            return;
        }
        getGroundPosition(position_);

        switch (status_) {
        // If Dino is on the ground, it can jump or bend over
        case DinoStatus::ON_GROUND:
        {
            // reset double jump CD
            double_jump_used_ = false;
            switch (action) {
            case Action::BEND_OVER:
                status_ = DinoStatus::BEND_OVER;
                break;
            case Action::JUMP:
                status_ = DinoStatus::IN_AIR;
                jump(false);
                break;
            default:
                walk(screen, position_);
            }
            break;
        }
        // Dino can do nothing if it jumped and still in the air
        case DinoStatus::IN_AIR:
        {
            if (stiff(screen, position_)) {
                status_ = DinoStatus::ON_GROUND;
            }
            if (dino_skill_.type.quick_drop) {
                if (action == Action::BEND_OVER) {
                    is_falling_ = false;
                    status_ = DinoStatus::BEND_OVER;
                    break;
                }
            }
            if (dino_skill_.type.double_jump) {
                if (!double_jump_used_ && is_falling_ && action == Action::JUMP) {
                    is_falling_ = false;
                    double_jump_used_ = true;
                    jump(true);
                    break;
                }
            }
            break;
        }
        // Dino bend over
        case DinoStatus::BEND_OVER:
        {
            bendOver(screen, position_);
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
        updateBoundingBox();
        spdlog::debug("DinoAction: {}, DinoState: {}", DINO_ACTION_STR[Utils::toIndex(action)],
                      DINO_STATUS_STR[Utils::toIndex(status_)]);
    }

    void setDinoNewSkill(DinoSkillType type)
    {
        switch (type) {
        case DinoSkillType::QUICK_DROP:
        {
            dino_skill_.type.quick_drop = true;
            break;
        }
        case DinoSkillType::DOUBLE_JUMP:
        {
            dino_skill_.type.double_jump = true;
            break;
        }
        }
    }
    DinoSkillBag getDinoSkillBag() { return dino_skill_; }

    void reset()
    {
        auto render_cfg = RenderConfig::getInstance();
        render_cfg->dino_alive = true;
        walk_count_ = 0;
        bend_count_ = 0;
        status_ = DinoStatus::ON_GROUND;
        is_falling_ = false;
        dino_tick_ = 0;
        double_jump_used_ = false;
    }

 private:
    /**
     * @retval true, on ground; false, in the IN_AIR
     */
    bool stiff(std::shared_ptr<lgfx::LGFX_Sprite> &screen, Position_t &position)
    {
        auto render_cfg = RenderConfig::getInstance();
        // update dino 'y'
        position_.y = dino_anim_.getValue(render_cfg->last_ts);
        screen->pushGrayscaleImage(position.x, position.y, size.width, size.height,
                                   dino_state_map[Utils::toIndex(DinoState::STAND_LEFT)],
                                   render_cfg->depth, render_cfg->prev_background_color,
                                   render_cfg->background_color);

        // start fall down animation after reaching the top
        if (!is_falling_ && dino_anim_.isFinished(render_cfg->last_ts)) {
            dino_anim_.setPathCallback(lvgl::LVAnimPathType::EASE_IN);
            dino_anim_.setDuration(dino_anim_duration_ * 50 / render_cfg->game_speed);
            dino_anim_.setValues(position_.y, render_cfg->getBottomPaddingY()
                                                  - render_cfg->getMiddlePaddingHeight() * 0.39);
            dino_anim_.setCurrentValue(render_cfg->last_ts);
            is_falling_ = true;
            return false;
        }

        if (is_falling_ && dino_anim_.isFinished(render_cfg->last_ts)) {
            is_falling_ = false;
            return true;
        }

        return false;
    }

    void jump(bool double_jump)
    {
        auto render_cfg = RenderConfig::getInstance();
        dino_anim_.setPathCallback(lvgl::LVAnimPathType::EASE_OUT);
        dino_anim_.setDuration(dino_anim_duration_);
        // jump in the air
        if (double_jump) {
            dino_anim_.setValues(position_.y, render_cfg->getMiddlePaddingY()
                                                  + render_cfg->getMiddlePaddingHeight() * 0.2);
        } else {  // jump from the ground
            dino_anim_.setValues(
                render_cfg->getBottomPaddingY() - render_cfg->getMiddlePaddingHeight() * 0.39,
                render_cfg->getMiddlePaddingY() + render_cfg->getMiddlePaddingHeight() * 0.2);
        }
        dino_anim_.setCurrentValue(render_cfg->last_ts);
    }

    void bendOver(std::shared_ptr<lgfx::LGFX_Sprite> &screen, Position_t position)
    {
        auto render_cfg = RenderConfig::getInstance();
        if (render_cfg->last_ts - dino_tick_ > render_cfg->getUpdateInterval()) {
            bend_count_ = (bend_count_ + 1) % BEND_STATE_NUM;
            dino_tick_ = render_cfg->last_ts;
        }
        screen->pushGrayscaleImage(position.x, position.y, size.width, size.height,
                                   dino_state_map[bend_count_ + 4], render_cfg->depth,
                                   render_cfg->prev_background_color, render_cfg->background_color);
    }

    void walk(std::shared_ptr<lgfx::LGFX_Sprite> &screen, Position_t position)
    {
        auto render_cfg = RenderConfig::getInstance();
        if (render_cfg->last_ts - dino_tick_ > render_cfg->getUpdateInterval()) {
            walk_count_ = (walk_count_ + 1) % STAND_STATE_NUM;
            dino_tick_ = render_cfg->last_ts;
        }
        screen->pushGrayscaleImage(position.x, position.y, size.width, size.height,
                                   dino_state_map[walk_count_], render_cfg->depth,
                                   render_cfg->prev_background_color, render_cfg->background_color);
    }

    void getGroundPosition(const Position_t &pos)
    {
        auto render_cfg = RenderConfig::getInstance();
        position_.x = render_cfg->screen_width * 0.1;
        position_.y = render_cfg->getBottomPaddingY() - render_cfg->getMiddlePaddingHeight() * 0.39;
    }

    void updateBoundingBox()
    {
        int32_t bound_height
            = status_ == DinoStatus::BEND_OVER ? size.height - size.bonding_offset : size.height;
        bounding_box_.upper_left.x = position_.x;
        bounding_box_.upper_left.y
            = status_ == DinoStatus::BEND_OVER ? position_.y + size.bonding_offset : position_.y;
        bounding_box_.lower_right.x = bounding_box_.upper_left.x + size.width;
        bounding_box_.lower_right.y = bounding_box_.upper_left.y + bound_height;

        // TODO(HangX-Ma): debug usage, draw box
        // screen->drawRect(bounding_box_.upper_left.x, bounding_box_.upper_left.y,
        // dino_size_.width,
        //                  bound_height, lgfx::colors::TFT_RED);
    }

 private:
    uint8_t walk_count_{0};
    uint8_t bend_count_{0};

    DinoStatus status_{DinoStatus::ON_GROUND};
    Position_t position_;
    BoundingBox_t bounding_box_;

    bool is_falling_{false};
    uint32_t dino_tick_{0};

    lvgl::LVAnim dino_anim_;
    uint32_t dino_anim_duration_{200};

    DinoSkillBag dino_skill_{0};
    bool double_jump_used_{false};
};
}  // namespace dino

#endif  // INCLUDE_DINO_DINO_H_
