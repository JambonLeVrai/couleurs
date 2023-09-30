#ifndef EFFECT_H
#define EFFECT_H

class Effect {
    public:
        Effect() {}
        virtual void refresh() {}
        virtual uint32_t get_color(Vec2f pos) { return 0; }

    protected:

};

#endif