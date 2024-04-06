#ifndef INCLUDE_DINO_OBSTACLE_MANAGER_H_
#define INCLUDE_DINO_OBSTACLE_MANAGER_H_

#include <memory>
#include "dino/bird.h"
#include "dino/cactus.h"
#include "dino/random.h"
#include "spdlog/spdlog.h"

namespace dino
{

enum class ObstacleType
{
    BIRD,
    CACTUS,
};

class ObstacleManager
{
 public:
    ObstacleManager()
    {
        cactus_factory_ = std::make_unique<CactusFactory>();
        auto cactus_type = cactus_factory_->getCactusType(CactusEnum::SMALL);
        cactus_ = std::make_unique<Cactus>(-0xFFFF, 0, cactus_type);
        bird_ = std::make_unique<Bird>();
    }

    void update(std::shared_ptr<lgfx::LGFX_Sprite> screen, uint32_t score)
    {
        auto render_cfg = RenderConfig::getInstance();
        // Skip update if time elapse shorter than random delay
        if (render_cfg->last_ts - obstacle_tick_ < random_delay_) {
            return;
        }

        if (score < render_cfg->bird_come_score) {
            is_finished_ = updateCactus(screen);

            if (checkFinished()) {
                random_delay_
                    = random_generator_.nextDouble(0.2, 1) * render_cfg->obstacle_max_delay;
                obstacle_tick_ = render_cfg->last_ts;
                spdlog::debug("random delay {}", random_delay_);
            }
        } else {
            switch (obstacle_type_) {
            case ObstacleType::BIRD:
                is_finished_ = updateBird(screen);
                break;
            case ObstacleType::CACTUS:
                is_finished_ = updateCactus(screen);
                break;
            }

            if (checkFinished()) {
                if (random_generator_.nextDouble(0, 1) > 0.3) {
                    obstacle_type_ = ObstacleType::CACTUS;
                } else {
                    obstacle_type_ = ObstacleType::BIRD;
                }
                random_delay_ = random_generator_.nextDouble(0, 1) * render_cfg->obstacle_max_delay;
                obstacle_tick_ = render_cfg->last_ts;
                spdlog::debug("random delay {}", random_delay_);
            }
        }
    }

    void reset()
    {
        obstacle_type_ = ObstacleType::CACTUS;
        // reset cactus
        cactus_.reset();
        cactus_ = std::make_unique<Cactus>(Position_t{-0xFFFF, 0},
                                           cactus_factory_->getCactusType(CactusEnum::SMALL));
        // reset bird
        bird_->resetState();
        is_finished_ = false;
        random_delay_ = 0;
        obstacle_tick_ = 0;
    }

    BoundingBox_t getBoundingBox()
    {
        switch (obstacle_type_) {
        case ObstacleType::BIRD:
            return bird_->getBoundingBox();
        case ObstacleType::CACTUS:
            return cactus_->getBoundingBox();
        }
        throw std::invalid_argument("Invalid obstacle type");
    }

 private:
    bool updateBird(std::shared_ptr<lgfx::LGFX_Sprite> &screen);
    bool updateCactus(std::shared_ptr<lgfx::LGFX_Sprite> &screen);

    bool checkFinished()
    {
        bool retval = is_finished_;
        is_finished_ = false;
        return retval;
    }

 private:
    std::unique_ptr<Cactus> cactus_;
    std::unique_ptr<CactusFactory> cactus_factory_;
    std::unique_ptr<Bird> bird_;

 private:
    ObstacleType obstacle_type_{ObstacleType::CACTUS};
    uint32_t obstacle_tick_{0};

    bool is_finished_{false};

    uint32_t random_delay_{0};
    Random random_generator_;
};
}  // namespace dino

#endif  // INCLUDE_DINO_OBSTACLE_MANAGER_H_
