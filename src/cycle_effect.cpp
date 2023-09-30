#include "cycle_effect.h"

CycleEffect::CycleEffect(std::vector<ColorF>* colors, float color_duration): _colors(colors), _color_duration(color_duration) {}

void CycleEffect::refresh() {
    float current_val = (float)millis() / _color_duration;
    int first_color_id = (int)current_val % _colors->size();
    int second_color_id = ((int)current_val + 1) % _colors->size();
    ColorF color1 = _colors->at(first_color_id);
    ColorF color2 = _colors->at(second_color_id);
    float t = current_val - floorf(current_val);
    current_color = color1 * (1-t) + color2 * t;
}

uint32_t CycleEffect::get_color(Vec2f pos) {
    return color_to_rgb(current_color);
}