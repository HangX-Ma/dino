#include "dino/obstacle_manager.h"

namespace dino
{

bool ObstacleManager::updateBird(std::shared_ptr<lgfx::LGFX_Sprite> &screen)
{
    bool retval = bird_->update(screen);
    auto render_cfg = RenderConfig::getInstance();
    if (render_cfg->dino_alive) {
        BirdEnum next_bird_enum;
        if (bird_->checkTickUpdate()) {
            next_bird_enum = static_cast<BirdEnum>((Utils::toIndex(bird_->getBirdState()) + 1)
                                                   % Utils::toIndex(BirdEnum::MAX_BIRD_STATE));
            bird_->setBirdState(next_bird_enum);
        }
    }
    return retval;
}

bool ObstacleManager::updateCactus(std::shared_ptr<lgfx::LGFX_Sprite> &screen)
{
    auto [pos, state] = cactus_->update(screen);
    if (state) {
        auto render_cfg = RenderConfig::getInstance();
        CactusEnum cactus_enum;
        // Low game speed only use 2 cactus enum: MIDDLE and SMALL
        if (render_cfg->game_speed < 18) {
            cactus_enum = static_cast<CactusEnum>(random_generator_.next(1, 2));
        } else {
            cactus_enum = static_cast<CactusEnum>(random_generator_.next(0, 2));
        }
        // release previous cactus resources
        auto cactus_type = CactusFactory::getCactusType(cactus_enum);
        cactus_.reset();
        cactus_ = std::make_unique<Cactus>(pos.x, pos.y, cactus_type);
        cactus_->update(screen);
        return true;
    }
    return false;
}
}  // namespace dino
