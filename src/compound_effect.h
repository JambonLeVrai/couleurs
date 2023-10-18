#ifndef COMPOUND_EFFECT_H
#define COMPOUND_EFFECT_H

#include "tools.h"
#include "effect.h"
#include <ArduinoSTL.h>

struct CompoundEffectData {
    uint8_t current_effect_type;
    uint8_t current_effect;
    uint8_t nb_effects;
    uint8_t local_byte;
    std::vector<Effect*> effects;
};

class CompoundEffect: public Effect {
    public:
    CompoundEffect(std::vector<Effect*> effects);
    ~CompoundEffect();
    void refresh();
    uint32_t get_color(Vec2f pos);

    private:
    std::vector<Effect*> _effects;
};

#endif