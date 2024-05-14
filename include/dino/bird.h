#ifndef INCLUDE_DINO_BIRD_H_
#define INCLUDE_DINO_BIRD_H_

#include <memory>
#include <utility>
#include <unordered_map>
#include "dino/obstacle.h"
#include "dino/utils.h"
#include "dino/common.h"
#include "lgfx/v1/LGFX_Sprite.hpp"

#include "assets/bird1.h"
#include "assets/bird2.h"

namespace dino
{

enum class BirdEnum : uint8_t
{
    WING_DOWN = 0,
    WING_UP,
    MAX_BIRD_STATE,
};

static const uint8_t *bird_state_map[2] = {IMAGE_DATA_BIRD1, IMAGE_DATA_BIRD2};

class Bird : public Obstacle
{
 public:
    explicit Bird(int width = 35, int height = 32, int offset = 5,
                  BirdEnum estate = BirdEnum::WING_DOWN)
        : Obstacle(width, height, offset), estate(estate)
    {
    }
    Bird(const ObstacleSize_t &size, BirdEnum estate) : Obstacle(size), estate(estate) {}
    bool update(std::shared_ptr<lgfx::LGFX_Sprite> &screen)
    {
        auto render_cfg = RenderConfig::getInstance();
        auto new_position
            = Position_t{render_cfg->game_speed,
                         static_cast<int32_t>(render_cfg->getMiddlePaddingY()
                                              + render_cfg->getMiddlePaddingHeight() * 0.42)};
        return Obstacle::update(screen, position, new_position,
                                bird_state_map[Utils::toIndex(estate)]);
    }

    void setPosition(Position_t new_position) { position = new_position; }
    void setBirdState(BirdEnum new_state) { estate = new_state; }
    BirdEnum getBirdState() const { return estate; }

    void resetState() override
    {
        Obstacle::resetState();
        estate = BirdEnum::WING_DOWN;
        position = {-0xFFFF, 0};
    }

 protected:
    BirdEnum estate;
    Position_t position{-0xFFFF, 0};
};

}  // namespace dino

#endif  // INCLUDE_DINO_BIRD_H_
