#ifndef INCLUDE_DINO_CACTUS_H_
#define INCLUDE_DINO_CACTUS_H_

#include <memory>
#include <stdexcept>
#include <utility>
#include <array>

#include "assets/cactus1.h"
#include "assets/cactus2.h"
#include "assets/cactus3.h"
#include "dino/common.h"
#include "dino/obstacle.h"
#include "dino/utils.h"
#include "lgfx/v1/LGFX_Sprite.hpp"

namespace dino
{

enum class CactusEnum : uint8_t
{
    LARGE,
    MIDDLE,
    SMALL,
    MAX_CACTUS_KIND,
};

inline bool operator<(CactusEnum lhs, CactusEnum rhs)
{
    return static_cast<int>(lhs) < static_cast<int>(rhs);
}

#define ENUM_TO_VALUE(e) static_cast<std::underlying_type_t<decltype(e)>>(e)
constexpr std::size_t CactusArraySize = ENUM_TO_VALUE(CactusEnum::MAX_CACTUS_KIND);

static const uint8_t *cactus_type_map[3]
    = {IMAGE_DATA_CACTUS_LARGE, IMAGE_DATA_CACTUS_MIDDLE, IMAGE_DATA_CACTUS_SMALL};

class CactusType : public Obstacle
{
 public:
    CactusType(int width, int height, CactusEnum etype) : Obstacle(width, height), etype(etype) {}
    CactusType(const ObstacleSize_t &size, CactusEnum etype) : Obstacle(size), etype(etype) {}

    bool update(std::shared_ptr<lgfx::LGFX_Sprite> &screen, Position_t &position)
    {
        auto render_cfg = RenderConfig::getInstance();
        auto new_position
            = Position_t{render_cfg->game_speed,
                         static_cast<int32_t>(render_cfg->getBottomPaddingY()
                                              - render_cfg->getMiddlePaddingHeight() * 0.3)};
        return Obstacle::update(screen, position, new_position,
                                cactus_type_map[Utils::toIndex(etype)]);
    }

 protected:
    CactusEnum etype;
};

class Cactus
{
 public:
    Cactus(int32_t x, int32_t y, std::shared_ptr<CactusType> type)
        : Cactus(Position_t{x, y}, type)
    {
    }

    Cactus(const Position_t &position, std::shared_ptr<CactusType> type)
        : cactus_type(type), position(position)
    {
    }

    const BoundingBox &getBoundingBox() const { return cactus_type->getBoundingBox(); }

    std::pair<Position_t, bool> update(std::shared_ptr<lgfx::LGFX_Sprite> &screen)
    {
        bool is_finished = cactus_type->update(screen, position);
        return {position, is_finished};
    }

 protected:
    std::shared_ptr<CactusType> cactus_type;
    Position_t position;
};

class CactusFactory
{
 public:
    static std::shared_ptr<CactusType> getCactusType(CactusEnum cactus_enum)
    {
        if (cactus_type_[ENUM_TO_VALUE(cactus_enum)] == nullptr) {
            ObstacleSize_t size;
            switch (cactus_enum) {
            case CactusEnum::LARGE:
                size.width = 51;
                size.height = 32;
                break;
            case CactusEnum::MIDDLE:
                size.width = 20;
                size.height = 32;
                break;
            case CactusEnum::SMALL:
                size.width = 16;
                size.height = 32;
                break;
            default:
                throw std::invalid_argument("Invalid cactus type");
            }
            auto new_type = std::make_shared<CactusType>(size, cactus_enum);
            cactus_type_[ENUM_TO_VALUE(cactus_enum)] = new_type;
            return new_type;
        }
        return cactus_type_[ENUM_TO_VALUE(cactus_enum)];
    }

 private:
    static std::array<std::shared_ptr<CactusType>, CactusArraySize> cactus_type_;
};
}  // namespace dino

#endif  // INCLUDE_DINO_CACTUS_H_
