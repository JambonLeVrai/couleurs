#include "circular_wave_effect.h"

CircularWaveEffect::CircularWaveEffect(CircularWaveEffectData data) : _wave_width(data.wave_width), _speed(data.speed), _duration(data.duration), _origin(Vec2f(data.origin_x, data.origin_y)), 
_wave_color(color_from_rgb(data.wave_r, data.wave_g, data.wave_b)), _fixed_color(color_from_rgb(data.fixed_r, data.fixed_g, data.fixed_b)), current_radius(0.) {
    max_radius = _speed * _duration;
}

CircularWaveEffect::CircularWaveEffect(float wave_width, float speed, float duration, Vec2f origin, ColorF wave_color, ColorF fixed_color): 
    _wave_width(wave_width*wave_width), _speed(speed), _duration(duration), _origin(origin), _wave_color(wave_color), _fixed_color(fixed_color), current_radius(0.) {
    max_radius = _speed * _duration;
}

void CircularWaveEffect::refresh() {
    float current_time = (float)millis() / 1000.;
    float progression = current_time / _duration;
    current_radius = (progression - floor(progression)) * max_radius;   
}

uint32_t CircularWaveEffect::get_color(Vec2f pos, uint8_t key_id) {
    float ring_dist = min(abs(Vec2f::sq_norm(pos - _origin) - current_radius), abs(Vec2f::sq_norm(pos - _origin) - current_radius + max_radius));
    float coeff = coeff_formula(ring_dist);
    ColorF actual_color = _wave_color * coeff + _fixed_color * (1 - coeff);
    return color_to_rgb(actual_color);
}

float CircularWaveEffect::coeff_formula(float distance) {
    return exp(-(distance*distance)/_wave_width);
}