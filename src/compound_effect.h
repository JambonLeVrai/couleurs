#ifndef COMPOUND_EFFECT_H
#define COMPOUND_EFFECT_H

#include "tools.h"
#include "effect.h"
#include <ArduinoSTL.h>

class CompoundEffect: public Effect {
    public:
    CompoundEffect(std::vector<Effect*> effects);
    void refresh();
    uint32_t get_color(Vec2f pos);

    private:
    std::vector<Effect*> _effects;
};

#endif