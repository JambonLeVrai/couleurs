#ifndef CIRCLUAR_WAVE_EFFECT_H
#define CIRCLUAR_WAVE_EFFECT_H

#include "tools.h"
#include "effect.h"

class CircularWaveEffect: public Effect {
    public:
    CircularWaveEffect(float wave_width, float speed, float duration, Vec2f origin, ColorF wave_color, ColorF fixed_color);
    void refresh();
    uint32_t get_color(Vec2f pos);
    float coeff_formula(float distance);

    private:
    Vec2f _origin;
    float _wave_width, _speed, _duration;
    float current_radius, max_radius;
    ColorF _fixed_color, _wave_color;
};

#endif