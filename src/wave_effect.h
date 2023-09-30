#ifndef WAVE_EFFECT_H
#define WAVE_EFFECT_H

#include "effect.h"
#include "tools.h"
#include <Adafruit_NeoPixel.h>
#include <math.h>

class WaveEffect: public Effect {
    public:
        WaveEffect(float prop_direction, float wavefront_direction, float wave_width, float speed, float duration, Vec2f rectangle_size, ColorF color);
        void refresh();
        uint32_t get_color(Vec2f pos);
        float coeff_formula(float distance);
        

    private:
        Vec2f prop_dir, wavefront_dir_90deg;
        Vec2f start_pos = Vec2f(0., 0.);
        Vec2f current_pos = Vec2f(0., 0.);
        float started_timer;
        ColorF _color;
        float width, _speed, _duration;
};

#endif