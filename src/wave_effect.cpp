#include "wave_effect.h"

WaveEffect::WaveEffect(WaveEffectData data): 
WaveEffect( 
    data.prop_direction, 
    data.wavefront_direction, 
    data.wave_width, 
    data.speed, 
    data.duration, 
    Vec2f(data.rectangle_size_x, data.rectangle_size_y), 
    color_from_rgb(data.wave_color_r, data.wave_color_g, data.wave_color_b),
    color_from_rgb(data.fixed_color_r, data.fixed_color_g, data.fixed_color_b)
)
{}

WaveEffect::WaveEffect(float prop_direction, float wavefront_direction, float wave_width, float speed, float duration, Vec2f rectangle_size, ColorF wave_color, ColorF fixed_color):
    prop_dir(vec_from_angle(prop_direction)), wavefront_dir_90deg(vec_from_angle(wavefront_direction + 90.)), _wave_color(wave_color), _fixed_color(fixed_color) {
    Vec2f total_travel = prop_dir * duration * speed;
    Serial.println(total_travel.x());
    Serial.println(total_travel.y());
    Vec2f center = rectangle_size / 2.;
    start_pos = center - total_travel / 2;
    current_pos = start_pos;

    _speed = speed;
    width = 2 * wave_width * wave_width;
    _duration = duration;
}

WaveEffect::WaveEffect(float prop_direction, float wavefront_direction, float wave_width, float speed, float duration, Vec2f rectangle_size, ColorF wave_color):
    WaveEffect(prop_direction, wavefront_direction, wave_width, speed, duration, rectangle_size, wave_color, ColorF(0., 0., 0.)) {}

WaveEffect::~WaveEffect() {}

void WaveEffect::refresh() {
    float current_time = (float)millis() / 1000.;
    float progression = current_time / _duration;
    float actual_distance = (progression - floorf(progression)) * _speed * _duration;
    current_pos = start_pos + (prop_dir * actual_distance);
}

float WaveEffect::coeff_formula(float distance) {
    return exp(-(distance*distance)/width);
}

uint32_t WaveEffect::get_color(Vec2f pos) {
    float distance_to_wavefront = abs(Vec2f::dot(pos - current_pos, wavefront_dir_90deg));
    float coeff = coeff_formula(distance_to_wavefront);
    ColorF actual_color = _wave_color * coeff + _fixed_color * (1-coeff);
    return color_to_rgb(actual_color);
}