#ifndef EFFECT_H
#define EFFECT_H

#include <Arduino.h>
#include "tools.h"

class Effect {
    public:
        Effect() {}
        virtual ~Effect() {}
        virtual void refresh() {}
        virtual ColorF get_color_vector(Vec2f pos) { return ColorF(0., 0., 0.); }
        virtual uint32_t get_color(Vec2f pos) { return 0; }

    protected:

};

#endif