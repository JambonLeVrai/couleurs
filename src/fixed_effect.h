#ifndef FIXED_EFFECT_H
#define FIXED_EFFECT_H

#include "effect.h"
#include "tools.h"

struct FixedEffectData {
    uint8_t R, G, B;
};

class FixedEffect: Effect {
    public: 
        FixedEffect(uint8_t R, uint8_t G, uint8_t B);
        FixedEffect(ColorF col);
        uint32_t get_color(Vec2f pos);

    private:
        uint32_t _col;
};

#endif