#include "persistence_effect.h"

PersistenceEffect::PersistenceEffect(PersistenceEffectData data): color(0., 0., 0.) {
    uint32_t current_time = millis();
    for(int i=0;i<LED_COUNT;i++) {
        PersistentKeyState ks;
        ks.state = false;
        ks.timer = current_time;
        timers.push_back(ks);
    }
    color = color_from_rgb(data.color.R, data.color.G, data.color.B);
    duration = data.persistence_duration;
}

uint32_t PersistenceEffect::get_color(Vec2f pos, uint8_t key_id) {
    // Special case if millis() overflows
    uint32_t t = millis();
    if(t <= timers[key_id].timer) {
        timers[key_id].timer = t;
        Serial.println("huh");
        return 0;
    }
    if(t - timers[key_id].timer <= duration) {
        float coef = 1. - (float)(t - timers[key_id].timer) / (float)duration;
        return color_to_rgb(color * coef);
    }
    return 0;
}

void PersistenceEffect::key_pressed(uint8_t key_id) {
    timers[key_id].state = true;
    timers[key_id].timer = millis();
}

void PersistenceEffect::key_released(uint8_t key_id) {
    timers[key_id].state = false;
}