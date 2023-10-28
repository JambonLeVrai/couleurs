#include <Adafruit_NeoPixel.h>
#include <ArduinoSTL.h>
#include <SPI.h>
#include "tools.h"
#include "wave_effect.h"
#include "cycle_effect.h"
#include "circular_wave_effect.h"
#include "compound_effect.h"
#include "SPI_processor.h"

#define PIN 2
#define LED_COUNT 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
SPIProcessor spi_processor;

uint8_t* buffer;
uint8_t buffer_length = 0;
uint8_t current_byte = 0;
volatile byte pos;
volatile boolean process_it;

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

// Effect* effect = new WaveEffect(-45., 45., 0.25, 3., 5., Vec2f(3., 3.), color_from_rgb(185, 65, 0));
/* Effect* effect = new CycleEffect(std::vector<ColorF>(new std::vector<ColorF>({color_from_rgb(185, 65, 0),
                                                       color_from_rgb(122, 0, 0),
                                                       color_from_rgb(0, 122, 0),
                                                       color_from_rgb(0, 0, 122),
                                                       color_from_rgb(40, 40, 40)}), 4000.);*/
// Effect* effect = new CircularWaveEffect(1., 5., 3., Vec2f(1.5, 0.5), color_from_rgb(185, 65, 0), color_from_rgb(0, 0, 0));
/*Effect* effect = new CompoundEffect(std::vector<Effect*>({
  new WaveEffect(-45., 45., 0.25, 3., 5., Vec2f(3., 3.), color_from_rgb(140, 0, 0)),
  new CircularWaveEffect(1., 5., 3., Vec2f(1.5, 0.5), color_from_rgb(0, 140, 20), color_from_rgb(0, 0, 0))
}));*/
Effect *effect = new WaveEffect(-45., 45., 0.5, 3., 5., Vec2f(3., 3.), color_from_rgb(140, 0, 0), color_from_rgb(0, 140, 0));
// Effect* effect = new FixedEffect(150, 72, 12);

void colorSet(uint32_t c, uint8_t wait);

// SPI interrupt routine
ISR(SPI_STC_vect)
{
    if (buffer_length == 0) {
        buffer_length = SPDR;
        buffer = new uint8_t[buffer_length]();
        Serial.println(buffer_length);
        current_byte = 0;
    }
    else {
        buffer[current_byte] = SPDR;
        current_byte++;
        if (current_byte == buffer_length) {
            process_it = true;
        }
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println("hihi");

    // have to send on master in, *slave out*
    pinMode(SS, INPUT_PULLUP);
    pinMode(MOSI, OUTPUT);
    pinMode(SCK, INPUT);
    SPCR |= _BV(SPE);
    SPI.attachInterrupt(); // allows SPI interrupt

    strip.begin();

    strip.show(); // Initialize all pixels to 'off'

    pos = 0;
    process_it = false;
}

void loop()
{
    effect->refresh();
    for (int i = 0; i < 8; i++)
    {
        strip.setPixelColor(i, effect->get_color(key_coords[i]));
    }
    strip.show();

    if (process_it)
    {
        for(int i=0;i<buffer_length;i++) {
            if(spi_processor.receiveData(buffer[i])) {
                delete effect;
                effect = spi_processor.getEffect();
            }
        }
        process_it = false;
        buffer_length = 0;
        delete[] buffer;
    }

    delay(5);
}