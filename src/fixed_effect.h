#ifndef FIXED_EFFECT_H
#define FIXED_EFFECT_H

#include "effect.h"
#include "tools.h"

class FixedEffect {
    public: 
        FixedEffect(ColorF col);
        uint32_t get_color(Vec2f pos);

    private:
        uint32_t _col;
};

#endif