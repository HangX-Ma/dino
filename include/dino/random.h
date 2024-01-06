#ifndef DINO_RANDOM_H
#define DINO_RANDOM_H

#include <random>
#include <stdexcept>

namespace dino
{
class Random
{
public:
    Random() : Random(std::random_device{}()) {}
    explicit Random(uint_least32_t seed) : random_generator_(seed) {}

    std::int32_t next() { return this->next(0, std::numeric_limits<std::int32_t>::max()); }

    std::int32_t next(std::int32_t max_value) { return this->next(0, max_value); }

    std::int32_t next(std::int32_t min_value, std::int32_t max_value)
    {
        if (min_value < 0 || max_value < min_value) {
            throw std::invalid_argument("minValue and maxValue must be non-negative. maxValue must "
                                        "be greater than minvalue");
        }
        std::uniform_int_distribution<std::int32_t> distribution(min_value, max_value);
        return distribution(this->random_generator_);
    }

    std::double_t nextDouble() { return this->nextDouble(0.0, 1.0); }

    std::double_t nextDouble(std::double_t min_value, std::double_t max_value)
    {
        if (min_value < 0.0 || max_value < min_value) {
            throw std::invalid_argument("minValue and maxValue must be non-negative. maxValue must "
                                        "be greater than minvalue");
        }
        std::uniform_real_distribution<std::double_t> distribution(min_value, max_value);
        return distribution(this->random_generator_);
    }

    std::double_t nextNormalDouble() { return this->nextNormalDouble(0.0, 1.0); }
    std::double_t nextNormalDouble(std::double_t expections, std::double_t variance)
    {
        std::normal_distribution<std::double_t> distribution(expections, variance);
        return distribution(this->random_generator_);
    }

private:
    std::mt19937 random_generator_;
};
} // namespace dino

#endif