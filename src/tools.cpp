#include "tools.h"


ColorF color_from_rgb(uint8_t R, uint8_t G, uint8_t B) {
    return ColorF((float)R / 255., (float)G / 255., (float)B / 255.);
}

ColorF color_from_rgb(ColorI c) {
    return color_from_rgb(c.R, c.G, c.B);
}

uint8_t saturate(int i) {
    if (i > 255)
        return 255;
    else if(i < 0)
        return 0;
    else
        return i;
}

uint32_t color_to_rgb(ColorF const& color) {
    int R = saturate((int)(color.x() * 255.));
    int G = saturate((int)(color.y() * 255.));
    int B = saturate((int)(color.z() * 255.));
    return Adafruit_NeoPixel::Color(R, G, B);
}

float deg_to_radians(float deg) {
    return deg * PI / 180.;
}

float radians_to_deg(float radians) {
    return radians * 180. / PI;
}

Vec2f vec_from_angle(float deg) {
    return Vec2f(
        cos(deg_to_radians(deg)),
        sin(deg_to_radians(deg))
    );
}