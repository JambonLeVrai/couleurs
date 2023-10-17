#ifndef SPI_PROCESSOR_H
#define SPI_PROCESSOR_H

#include <Arduino.h>
#include "effect.h"
#include "cycle_effect.h"
#include "compound_effect.h"
#include "circular_wave_effect.h"
#include "fixed_effect.h"
#include "wave_effect.h"

#define EMPTY_COMMAND 0
#define COMMAND_SET_NEW_EFFECT 1
#define COMMAND_KEY_PRESSED 2
#define COMMAND_KEY_RELEASED 3

#define EMPTY_EFFECT 0
#define EFFECT_FIXED 1
#define EFFECT_CYCLE 2
#define EFFECT_CIRCULAR_WAVE 3
#define EFFECT_WAVE 4
#define EFFECT_COMPOUND 5

class SPIProcessor {
    public:
    SPIProcessor();
    bool receiveData(uint8_t byte);
    bool setNewEffect(uint8_t byte);
    bool processFixedEffect(uint8_t byte);
    bool processCircularWaveEffect(uint8_t byte);
    bool processCycleEffect(uint8_t byte);

    Effect* getEffect();

    private:
    uint8_t current_command;
    uint8_t effect;
    uint8_t current_byte;
    uint8_t state;
    FixedEffectData current_fixed_effect_data;
    CycleEffectData current_cycle_effect_data;
    CircularWaveEffectData current_circular_wave_effect_data;
    WaveEffectData current_wave_effect_data;
};

#endif