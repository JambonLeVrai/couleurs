#ifndef SPI_PROCESSOR_H
#define SPI_PROCESSOR_H

#include <Arduino.h>
#include "effect.h"
#include "cycle_effect.h"
#include "compound_effect.h"
#include "circular_wave_effect.h"
#include "fixed_effect.h"
#include "wave_effect.h"
#include "fixed_pattern_effect.h"
#include "persistence_effect.h"

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
#define EFFECT_FIXED_PATTERN 6
#define EFFECT_PERSISTENCE 7

enum class ProcessedType {
    NONE, NEW_EFFECT, KEY_PRESSED, KEY_RELEASED
};

class SPIProcessor {
    public:
    SPIProcessor();
    ProcessedType receiveData(uint8_t byte);
    bool setNewEffect(uint8_t byte);
    bool processFixedEffect(uint8_t byte, uint8_t& current);
    bool processCircularWaveEffect(uint8_t byte, uint8_t& current);
    bool processWaveEffect(uint8_t byte, uint8_t& current);
    bool processCycleEffect(uint8_t byte, uint8_t& current);
    bool processCompoundEffect(uint8_t byte);
    bool processFixedPatternEffect(uint8_t byte, uint8_t& current);
    bool processPersistenceEffect(uint8_t byte, uint8_t& current);

    bool setKeyPressed(uint8_t byte);
    bool setKeyReleased(uint8_t byte);

    Effect* getEffect();
    Effect* getRawEffect(uint8_t effect_type);

    uint8_t keyPressed();
    uint8_t keyReleased();

    private:
    uint8_t current_command;
    uint8_t effect;
    uint8_t current_byte;
    uint8_t state;

    uint8_t pressed_key_code;
    uint8_t released_key_code;

    FixedEffectData current_fixed_effect_data;
    CycleEffectData current_cycle_effect_data;
    CircularWaveEffectData current_circular_wave_effect_data;
    WaveEffectData current_wave_effect_data;
    CompoundEffectData current_compound_effect_data;
    FixedPatternEffectData current_fixed_pattern_effect_data;
    PersistenceEffectData current_persistence_effect_data;
};

#endif