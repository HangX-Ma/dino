#include "dino/obstacle.h"

namespace dino
{

bool Obstacle::updateBird(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg)
{
    return bird_.update(screen, render_cfg);
}

bool Obstacle::updateCactus(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg)
{
    if (cactus_.update(screen, render_cfg, cactus_type_)) {
        // Low game speed only use 2 cactus type: MIDDLE and SMALL
        if (render_cfg.game_speed < 42) {
            cactus_type_ = static_cast<CactusType>(random_generator_.next(1, 2));
        }
        else {
            cactus_type_ = static_cast<CactusType>(random_generator_.next(0, 2));
        }
        return true;
    }
    return false;
}
} // namespace dino