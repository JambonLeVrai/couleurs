#ifndef FIXED_EFFECT_H
#define FIXED_EFFECT_H

#include "effect.h"
#include "tools.h"

struct FixedEffectData {
    uint8_t R, G, B;
} __attribute__ ((packed));

class FixedEffect: Effect {
    public: 
        FixedEffect(FixedEffectData data);
        FixedEffect(ColorF col);
        uint32_t get_color(Vec2f pos, uint8_t key_id);

    private:
        uint32_t _col;
};

#endif