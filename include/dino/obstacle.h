#ifndef DINO_OBSTACLE_H
#define DINO_OBSTACLE_H

#include "SDL_joystick.h"
#include "dino/bird.h"
#include "dino/cactus.h"
#include "dino/random.h"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"

namespace dino
{

enum class ObstacleType
{
    BIRD,
    CACTUS,
};

class Obstacle
{
public:
    void update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, uint32_t score,
                bool dino_alive)
    {
        if (render_cfg.last_ts - obstacle_tick_ < random_delay_) {
            return;
        }

        if (score < render_cfg.bird_come_score) {
            is_finished_ = updateCactus(screen, render_cfg, dino_alive);

            if (is_finished_) {
                is_finished_ = false;
                random_delay_
                    = random_generator_.nextDouble(0.2, 1) * render_cfg.obstacle_max_delay;
                obstacle_tick_ = render_cfg.last_ts;
                spdlog::debug("random delay {}", random_delay_);
            }
        }
        else {
            switch (obstacle_type_) {
            case ObstacleType::BIRD:
                is_finished_ = updateBird(screen, render_cfg, dino_alive);
                break;
            case ObstacleType::CACTUS:
                is_finished_ = updateCactus(screen, render_cfg, dino_alive);
                break;
            }

            if (is_finished_) {
                if (random_generator_.nextDouble(0, 1) > 0.3) {
                    obstacle_type_ = ObstacleType::CACTUS;
                }
                else {
                    obstacle_type_ = ObstacleType::BIRD;
                }
                is_finished_ = false;
                random_delay_ = random_generator_.nextDouble(0, 1) * render_cfg.obstacle_max_delay;
                obstacle_tick_ = render_cfg.last_ts;
                spdlog::debug("random delay {}", random_delay_);
            }
        }
    }

    void reset()
    {
        cactus_type_ = CactusType::SMALL;
        obstacle_type_ = ObstacleType::CACTUS;
        is_finished_ = false;
        random_delay_ = 0;
        obstacle_tick_ = 0;

        bird_.reset();
        cactus_.reset();
    }

    BoundingBox_t getBoundingBox()
    {
        switch (obstacle_type_) {
        case ObstacleType::BIRD:
            return bird_.getBoundingBox();
        case ObstacleType::CACTUS:
            return cactus_.getBoundingBox();
        }
        throw std::invalid_argument("Invalid obstacle type");
    }

private:
    bool updateBird(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, bool dino_alive);
    bool updateCactus(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, bool dino_alive);

private:
    Bird bird_;
    Cactus cactus_{};
    CactusType cactus_type_{CactusType::SMALL};
    ObstacleType obstacle_type_{ObstacleType::CACTUS};
    bool is_finished_{false};
    uint32_t random_delay_{0};
    uint32_t obstacle_tick_{0};

    Random random_generator_;
};
} // namespace dino

#endif
