#ifndef WAVE_EFFECT_H
#define WAVE_EFFECT_H

#include "effect.h"
#include "tools.h"
#include <Adafruit_NeoPixel.h>
#include <math.h>

struct WaveEffectData {
    float prop_direction;
    float wavefront_direction;
    float wave_width;
    float speed;
    float duration;
    float rectangle_size_x;
    float rectangle_size_y;
    uint8_t wave_color_r;
    uint8_t wave_color_g;
    uint8_t wave_color_b;
    uint8_t fixed_color_r;
    uint8_t fixed_color_g;
    uint8_t fixed_color_b;
} __attribute__ ((packed));

class WaveEffect: public Effect {
    public:
        WaveEffect(WaveEffectData data);
        WaveEffect(float prop_direction, float wavefront_direction, float wave_width, float speed, float duration, Vec2f rectangle_size, ColorF wave_color, ColorF fixed_color);
        WaveEffect(float prop_direction, float wavefront_direction, float wave_width, float speed, float duration, Vec2f rectangle_size, ColorF wave_color);
        ~WaveEffect();
        void refresh();
        uint32_t get_color(Vec2f pos);
        float coeff_formula(float distance);
        

    private:
        Vec2f prop_dir, wavefront_dir_90deg;
        Vec2f start_pos = Vec2f(0., 0.);
        Vec2f current_pos = Vec2f(0., 0.);
        float started_timer;
        ColorF _wave_color, _fixed_color;
        float width, _speed, _duration;
};

#endif