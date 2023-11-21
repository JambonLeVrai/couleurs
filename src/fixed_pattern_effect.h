#ifndef FIXED_PATTERN_EFFECT_H
#define FIXED_PATTERN_EFFECT_H

#include "effect.h"
#include "tools.h"

using FixedPatternEffectData = std::vector<ColorI>;

class FixedPatternEffect: Effect {
    public: 
        FixedPatternEffect(FixedPatternEffectData data);
        uint32_t get_color(Vec2f pos, uint8_t key_id);

    private:
        std::vector<uint32_t> colors;
};

#endif