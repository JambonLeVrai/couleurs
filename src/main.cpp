#include <Adafruit_NeoPixel.h>
#include <ArduinoSTL.h>
#include "tools.h"
#include "wave_effect.h"
#include "cycle_effect.h"
#include "circular_wave_effect.h"
#include "compound_effect.h"

#define PIN 2
#define LED_COUNT 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

Vec2f key_coords[8] = {
  Vec2f(0., 0.),
  Vec2f(1., 0.),
  Vec2f(2., 0.),
  Vec2f(3., 0.),
  Vec2f(0., 1.),
  Vec2f(1., 1.),
  Vec2f(2., 1.),
  Vec2f(3., 1.),
};

std::vector<ColorF>* colors = new std::vector<ColorF>({
  color_from_rgb(185, 65, 0),
  color_from_rgb(122, 0, 0),
  color_from_rgb(0, 122, 0),
  color_from_rgb(0, 0, 122),
  color_from_rgb(40, 40, 40)
});

//Effect* effect = new WaveEffect(-45., 45., 0.25, 3., 5., Vec2f(3., 3.), color_from_rgb(185, 65, 0));
//Effect* effect = new CycleEffect(colors, 4000.);
//Effect* effect = new CircularWaveEffect(1., 5., 3., Vec2f(1.5, 0.5), color_from_rgb(185, 65, 0), color_from_rgb(0, 0, 0));
Effect* effect = new CompoundEffect(std::vector<Effect*>({
  new WaveEffect(-45., 45., 0.25, 3., 5., Vec2f(3., 3.), color_from_rgb(140, 0, 0)),
  new CircularWaveEffect(1., 5., 3., Vec2f(1.5, 0.5), color_from_rgb(0, 140, 20), color_from_rgb(0, 0, 0))
}));
//Effect* effect = new WaveEffect(-45., 45., 0.25, 3., 5., Vec2f(3., 3.), color_from_rgb(140, 0, 0));

void colorSet(uint32_t c, uint8_t wait);

void setup() {
  Serial.begin(9600);
  strip.begin();

  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  effect->refresh();
  for(int i=0;i<8;i++) {
    strip.setPixelColor(i, effect->get_color(key_coords[i]));
  }
  strip.show();
  delay(50.);
}