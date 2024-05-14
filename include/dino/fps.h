#ifndef INCLUDE_DINO_FPS_H_
#define INCLUDE_DINO_FPS_H_

#include "dino/utils.h"
#include <string>
#include "spdlog/spdlog.h"

namespace dino
{

class FPS
{
 public:
    const std::string &update()
    {
        if (Utils::getTimestamp() - time_stamp_ > 1000 /*ms*/) {
            time_stamp_ = Utils::getTimestamp();
            fps_ = frame_;
            frame_ = 0;
        }
        frame_ += 1;
        text_ = std::string("FPS: ").append(std::to_string(fps_));

        return text_;
    }

 private:
    uint32_t fps_{0};
    uint32_t time_stamp_{0};
    uint32_t frame_{0};
    std::string text_;
};

}  // namespace dino

#endif  // INCLUDE_DINO_FPS_H_
