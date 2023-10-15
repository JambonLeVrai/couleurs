#include "fixed_effect.h"

FixedEffect::FixedEffect(uint8_t R, uint8_t G, uint8_t B): _col(Adafruit_NeoPixel::Color(R, G, B)) {}

FixedEffect::FixedEffect(ColorF col): _col(color_to_rgb(col)) {}

uint32_t FixedEffect::get_color(Vec2f pos) {
    return _col;
}