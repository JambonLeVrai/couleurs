#ifndef CYCLE_EFFECT_H
#define CYCLE_EFFECT_H

#include "tools.h"
#include "effect.h"
#include <ArduinoSTL.h>

class CycleEffect: public Effect {
    public:
        CycleEffect(std::vector<ColorF>* colors, float color_duration);
        void refresh();
        uint32_t get_color(Vec2f pos);

    private:
        float _color_duration;
        std::vector<ColorF>* _colors;
        ColorF current_color = ColorF(0., 0., 0.);
};

#endif