#include "SPI_processor.h"

SPIProcessor::SPIProcessor() {}

void SPIProcessor::processKeyPressed(uint8_t* buff) {
    pressed_key_code = *buff;
}

void SPIProcessor::processKeyReleased(uint8_t* buff) {
    released_key_code = *buff;
}

uint8_t SPIProcessor::keyPressed() {
    return pressed_key_code;
}

uint8_t SPIProcessor::keyReleased() {
    return released_key_code;
}

Effect* SPIProcessor::getEffect() {
    return effect_holder;
}

ProcessedType SPIProcessor::processData(uint8_t* buff) {
    uint8_t current_command = *buff;
    buff++;
    switch(current_command) {
        case COMMAND_SET_NEW_EFFECT:
            processNewEffect(buff);
            return ProcessedType::NEW_EFFECT;
            break;
        case COMMAND_KEY_PRESSED:
            processKeyPressed(buff);
            return ProcessedType::KEY_PRESSED;
            break;
        case COMMAND_KEY_RELEASED:
            processKeyReleased(buff);
            return ProcessedType::KEY_RELEASED;
            break;
    }
    return ProcessedType::NONE;
}

uint8_t* SPIProcessor::processNewEffect(uint8_t* buff) {
    uint8_t effect_type = *buff;
    buff++;
    switch(effect_type) {
        case EFFECT_FIXED:
            return processFixedEffect(buff);
            break;

        case EFFECT_CYCLE:
            return processCycleEffect(buff);
            break;
        
        case EFFECT_WAVE:
            return processWaveEffect(buff);
            break;

        case EFFECT_CIRCULAR_WAVE:
            return processCircularWaveEffect(buff);
            break;

        case EFFECT_COMPOUND:
            return processCompoundEffect(buff);
            break;

        case EFFECT_FIXED_PATTERN:
            return processFixedPatternEffect(buff);
            break;

        case EFFECT_PERSISTENCE:
            return processPersistenceEffect(buff);
            break;
    }
}

uint8_t* SPIProcessor::processFixedEffect(uint8_t *buff) {
    FixedEffectData data;
    memcpy(&data, buff, sizeof(FixedEffectData));
    effect_holder = (Effect*)new FixedEffect(data);
    return buff + sizeof(FixedEffectData);
}

uint8_t* SPIProcessor::processCircularWaveEffect(uint8_t *buff) {
    CircularWaveEffectData data;
    memcpy(&data, buff, sizeof(CircularWaveEffectData));
    effect_holder = (Effect*)new CircularWaveEffect(data);
    return buff + sizeof(CircularWaveEffectData);
}

uint8_t* SPIProcessor::processWaveEffect(uint8_t *buff) {
    WaveEffectData data;
    memcpy(&data, buff, sizeof(WaveEffectData));
    effect_holder = (Effect*)new WaveEffect(data);
    return buff + sizeof(WaveEffectData);
}

uint8_t* SPIProcessor::processCycleEffect(uint8_t *buff) {
    CycleEffectData data;
    data.nb_colors = *(buff++);
    data.colors = new std::vector<ColorF>();
    memcpy(&data.color_duration, buff, 4);
    buff += 4;
    for(int i=0;i<data.nb_colors;i++) {
        ColorF c = color_from_rgb(*buff, *(buff+1), *(buff+2));
        data.colors->push_back(c);
        buff += 3;
    }
    effect_holder = (Effect*)new CycleEffect(data.colors, data.color_duration);
    return buff;
}

uint8_t* SPIProcessor::processCompoundEffect(uint8_t *buff) {
    uint8_t nb_effects;
    std::vector<Effect*> effects_vec;
    nb_effects = *(buff++);
    for(int i=0;i<nb_effects;i++) {
        buff = processNewEffect(buff);
        effects_vec.push_back(effect_holder);
    }
    effect_holder = (Effect*)new CompoundEffect(effects_vec);
    return buff;
}

uint8_t* SPIProcessor::processFixedPatternEffect(uint8_t *buff) {
    FixedPatternEffectData data;
    for(int i=0;i<KEYS;i++) {
        ColorI c;
        memcpy(&c, buff, 3);
        data.push_back(c);
        buff += 3;
    }
    effect_holder = (Effect*)new FixedPatternEffect(data);
    return buff + 3*KEYS;
}

uint8_t* SPIProcessor::processPersistenceEffect(uint8_t *buff) {
    PersistenceEffectData data;
    memcpy(&data, buff, sizeof(PersistenceEffectData));
    effect_holder = (Effect*) new PersistenceEffect(data);
    return buff + sizeof(PersistenceEffectData);
}