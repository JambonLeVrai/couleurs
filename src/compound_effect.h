#ifndef COMPOUND_EFFECT_H
#define COMPOUND_EFFECT_H

#include "tools.h"
#include "effect.h"

struct CompoundEffectData {
    uint8_t current_effect_type;
    uint8_t current_effect;
    uint8_t nb_effects;
    uint8_t local_byte;
    std::vector<Effect*> effects;
} __attribute__ ((packed));

class CompoundEffect: public Effect {
    public:
    CompoundEffect(std::vector<Effect*> effects);
    ~CompoundEffect();
    void refresh();
    uint32_t get_color(Vec2f pos, uint8_t key_id);
    void key_pressed(uint8_t key_id);
    void key_released(uint8_t key_id);

    private:
    std::vector<Effect*> _effects;
};

#endif