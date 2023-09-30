#include "tools.h"


ColorF color_from_rgb(uint8_t R, uint8_t G, uint8_t B) {
    return ColorF((float)R / 255., (float)G / 255., (float)B / 255.);
}

uint32_t color_to_rgb(ColorF const& color) {
    int R = (int)(color.x() * 255.);
    int G = (int)(color.y() * 255.);
    int B = (int)(color.z() * 255.);
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