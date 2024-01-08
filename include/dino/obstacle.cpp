#include "dino/obstacle.h"

namespace dino
{

bool Obstacle::updateBird(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, bool dino_alive)
{
    return bird_.update(screen, render_cfg, dino_alive);
}

bool Obstacle::updateCactus(lgfx::LGFX_Sprite *screen, RenderConfig_t &render_cfg, bool dino_alive)
{
    if (cactus_.update(screen, render_cfg, cactus_type_, dino_alive)) {
        // Low game speed only use 2 cactus type: MIDDLE and SMALL
        if (render_cfg.game_speed < 30) {
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