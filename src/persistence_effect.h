#ifndef PERSISTENCE_EFFECT_H
#define PERSISTENCE_EFFECT_H

#include "effect.h"
#include "tools.h"

struct PersistenceEffectData {
    ColorI color;
    uint16_t persistence_duration;
} __attribute__ ((packed));

struct PersistentKeyState {
    uint32_t timer;
    bool state = false; // true = pressed / false = released
};

class PersistenceEffect: public Effect {
    public:
    PersistenceEffect(PersistenceEffectData data);
    uint32_t get_color(Vec2f pos, uint8_t key_id);
    void key_pressed(uint8_t key_id);
    void key_released(uint8_t key_id);

    private:
    std::vector<PersistentKeyState> timers;
    ColorF color;
    uint16_t duration;
};

#endif