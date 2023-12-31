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

    uint8_t* processFixedEffect(uint8_t *buff);
    uint8_t* processCircularWaveEffect(uint8_t *buff);
    uint8_t* processWaveEffect(uint8_t *buff);
    uint8_t* processCycleEffect(uint8_t *buff);
    uint8_t* processCompoundEffect(uint8_t *buff);
    uint8_t* processFixedPatternEffect(uint8_t *buff);
    uint8_t* processPersistenceEffect(uint8_t *buff);

    void processKeyPressed(uint8_t *buff);
    void processKeyReleased(uint8_t *buff);

    ProcessedType processData(uint8_t *buff);
    uint8_t* processNewEffect(uint8_t *buff);

    Effect* getEffect();

    uint8_t keyPressed();
    uint8_t keyReleased();

    private:
    uint8_t pressed_key_code;
    uint8_t released_key_code;

    Effect* effect_holder;
};

#endif