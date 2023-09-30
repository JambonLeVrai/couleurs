#include "compound_effect.h"

CompoundEffect::CompoundEffect(std::vector<Effect*> effects): _effects(effects) {}

void CompoundEffect::refresh() {
    for(auto& it: _effects)
        it->refresh();
}

uint32_t CompoundEffect::get_color(Vec2f pos) {
    ColorF result(0., 0., 0.);
    uint32_t this_color;
    for(auto& it: _effects) {
        this_color = it->get_color(pos);
        result += color_from_rgb((uint8_t)(this_color >> 16), (uint8_t)(this_color >> 8), (uint8_t)this_color);
    }
    return color_to_rgb(result);
}