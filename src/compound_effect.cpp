#include "compound_effect.h"

CompoundEffect::CompoundEffect(std::vector<Effect*> effects): _effects(effects) {}

CompoundEffect::~CompoundEffect() {
    for(auto& it: _effects)
        delete it;
}

void CompoundEffect::refresh() {
    for(auto& it: _effects)
        it->refresh();
}

uint32_t CompoundEffect::get_color(Vec2f pos, uint8_t key_id) {
    ColorF result(0., 0., 0.);
    uint32_t this_color;
    for(auto& it: _effects) {
        this_color = it->get_color(pos, key_id);
        result += color_from_rgb((uint8_t)(this_color >> 16), (uint8_t)(this_color >> 8), (uint8_t)this_color);
    }
    return color_to_rgb(result);
}

void CompoundEffect::key_pressed(uint8_t key_code) {
    for(auto& it: _effects) {
        it->key_pressed(key_code);
    }
}

void CompoundEffect::key_released(uint8_t key_code) {
    for(auto& it: _effects) {
        it->key_pressed(key_code);
    }
}