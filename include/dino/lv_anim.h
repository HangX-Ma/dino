#pragma once
#include <cstdint>
#include <type_traits>

namespace lvgl
{

enum class LVAnimPathType
{
    LINEAR,
    EASE_IN,
    EASE_OUT,
    EASE_IN_OUT,
    OVERSHOOT,
    BOUNCE,
    STEP,
    LVANIM_MAX_TYPE,
};

template <class type>
constexpr std::common_type_t<int, std::underlying_type_t<type>> toInteger(type val)
{
    return static_cast<std::common_type_t<int, std::underlying_type_t<type>>>(val);
}

using LVAnimPathCallBack_t = int32_t (*)(const struct LVAnimInner *);
using LVAnimInner_t = struct LVAnimInner
{
    int32_t start_value;   /**< Start value*/
    int32_t current_value; /**< Current value*/
    int32_t end_value;     /**< End value*/
    int32_t duration;      /**< Animation time in ms*/
    int32_t act_time;      /**< Current time in animation. Set to negative to make delay.*/
};

int32_t lvMap(int32_t x, int32_t min_in, int32_t max_in, int32_t min_out, int32_t max_out);
uint32_t lvBezier3(uint32_t t, uint32_t u0, uint32_t u1, uint32_t u2, uint32_t u3);

int32_t lvAnimPathLinear(const LVAnimInner_t *a);
int32_t lvAnimPathEaseIn(const LVAnimInner_t *a);
int32_t lvAnimPathEaseOut(const LVAnimInner_t *a);
int32_t lvAnimPathEaseInOut(const LVAnimInner_t *a);
int32_t lvAnimPathOvershoot(const LVAnimInner_t *a);
int32_t lvAnimPathBounce(const LVAnimInner_t *a);
int32_t lvAnimPathStep(const LVAnimInner_t *a);

static LVAnimPathCallBack_t path_cb_map[7] = {
    lvAnimPathLinear,    lvAnimPathEaseIn, lvAnimPathEaseOut, lvAnimPathEaseInOut,
    lvAnimPathOvershoot, lvAnimPathBounce, lvAnimPathStep,
};

class LVAnim
{
public:
    LVAnim()
        : lv_anim_inner_(LVAnimInner_t{
            .start_value = 0,
            .current_value = 0,
            .end_value = 0,
            .duration = 0,
            .act_time = 0,
        }),
          path_cb_(lvAnimPathEaseOut)
    {
    }
    void setDuration(uint32_t duration) { lv_anim_inner_.duration = duration; }

    void setDelay(uint32_t delay) { lv_anim_inner_.act_time = -static_cast<int32_t>(delay); }

    void setValues(int32_t start, int32_t end)
    {
        lv_anim_inner_.start_value = start;
        lv_anim_inner_.end_value = end;
    }

    void setPathCallback(LVAnimPathType type) { path_cb_ = path_cb_map[toInteger(type)]; }

    void setCurrentValue(int32_t current_time) { lv_anim_inner_.current_value = current_time; }

    int32_t getValue(int32_t current_time)
    {
        lv_anim_inner_.act_time = current_time - lv_anim_inner_.current_value;
        return path_cb_(&lv_anim_inner_);
    }

    bool isFinished(int32_t current_time)
    {
        return (current_time - lv_anim_inner_.current_value) > lv_anim_inner_.duration;
    }

private:
    LVAnimInner_t lv_anim_inner_;
    LVAnimPathCallBack_t path_cb_;
};

} // namespace lvgl