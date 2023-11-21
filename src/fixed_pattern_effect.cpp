#include "fixed_pattern_effect.h"

FixedPatternEffect::FixedPatternEffect(FixedPatternEffectData data) {
    for(auto& it: data) {
        colors.push_back(Adafruit_NeoPixel::Color(it.R, it.G, it.B));
    }
}

uint32_t FixedPatternEffect::get_color(Vec2f pos, uint8_t key_id) {
    return colors[key_id];
}