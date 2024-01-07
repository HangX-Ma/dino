#ifndef DINO_OBSTACLE_H
#define DINO_OBSTACLE_H

#include "dino/bird.h"
#include "dino/cactus.h"
#include "dino/random.h"
#include "lgfx/v1/misc/enum.hpp"

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
    void update(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, uint32_t score)
    {
        if (score < render_cfg.bird_come_score) {
            updateCactus(screen, render_cfg);
        }
        else {
            switch (obstacle_type_) {
            case ObstacleType::BIRD:
                is_finished_ = updateBird(screen, render_cfg);
                break;
            case ObstacleType::CACTUS:
                is_finished_ = updateCactus(screen, render_cfg);
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
            }
        }
    }

private:
    bool updateBird(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg);
    bool updateCactus(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg);

private:
    Bird bird_;
    Cactus cactus_{};
    CactusType cactus_type_{CactusType::SMALL};
    ObstacleType obstacle_type_{ObstacleType::BIRD};
    bool is_finished_{false};

    Random random_generator_;
};
} // namespace dino

#endif