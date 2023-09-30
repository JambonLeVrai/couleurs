#include "fixed_effect.h"

FixedEffect::FixedEffect(ColorF col): _col(color_to_rgb(col)) {}

uint32_t FixedEffect::get_color(Vec2f pos) {
    return _col;
}