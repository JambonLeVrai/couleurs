#include "SPI_processor.h"

SPIProcessor::SPIProcessor() {
    current_command = EMPTY_COMMAND;
    effect = EMPTY_EFFECT;
    current_byte = 0;
    state = 0;
}

bool SPIProcessor::receiveData(uint8_t byte) {
    if (current_command == EMPTY_COMMAND) {
        current_command = byte;
        return false;
    }

    switch(current_command) {
        case COMMAND_SET_NEW_EFFECT:
            return setNewEffect(byte);
            break;
        case COMMAND_KEY_PRESSED:

            break;
        case COMMAND_KEY_RELEASED:

            break;
    }

    return false;
}

bool SPIProcessor::processCycleEffect(uint8_t byte) {
    if(current_byte == 0) {
        current_cycle_effect_data.nb_colors = byte;
        current_cycle_effect_data.colors = new std::vector<ColorF>();
        current_byte++;
        return false;
    }
    if(current_byte <= 4 && current_byte >= 1) {
        *((uint8_t*)&current_cycle_effect_data.color_duration + (current_byte - 1)) = byte;
        current_byte++;
        return false;
    }

    if((current_byte-1) % 3 == 0) {
        current_cycle_effect_data.cur_r = byte;
        current_byte++;
        return false;
    }
    else if((current_byte-1) % 3 == 1) {
        current_cycle_effect_data.cur_g = byte;
        current_byte++;
        return false;
    }
    else {
        current_cycle_effect_data.cur_b = byte;
        current_byte++;
        current_cycle_effect_data.colors->push_back(color_from_rgb(current_cycle_effect_data.cur_r, current_cycle_effect_data.cur_g, current_cycle_effect_data.cur_b));

        return (current_byte-1) / 3 >= current_cycle_effect_data.nb_colors;
    }
}

bool SPIProcessor::setNewEffect(uint8_t byte) {
    if (effect == EMPTY_EFFECT) {
        effect = byte;
        current_byte = 0;
        return false;
    }

    bool result;
    switch(effect) {
        case EFFECT_FIXED:
            *((uint8_t*)(&current_fixed_effect_data) + current_byte) = byte;
            current_byte++;
            if (current_byte == sizeof(FixedEffectData)) {
                Serial.println("Received a whole fixed effect data");
                }
            return (current_byte == sizeof(FixedEffectData));
            break;

        case EFFECT_CYCLE:
            return processCycleEffect(byte);
            break;
        
        case EFFECT_WAVE:
            *((uint8_t*)(&current_wave_effect_data) + current_byte) = byte;
            current_byte++;
            if(current_byte == sizeof(WaveEffectData)) {
                Serial.println("Received a whole effect wave");
            }
            return (current_byte == sizeof(WaveEffectData));
            break;

        case EFFECT_CIRCULAR_WAVE:
            *((uint8_t*)(&current_circular_wave_effect_data) + current_byte) = byte;
            current_byte++;
            if(current_byte == sizeof(CircularWaveEffectData)) {
                Serial.println("Received a whole effect circular wave");
            }
            return (current_byte == sizeof(CircularWaveEffectData));
            break;
    }

}

Effect* SPIProcessor::getEffect() {
    Effect* result;
    switch(effect) {
        case EFFECT_FIXED:
            result = (Effect*)new FixedEffect(current_fixed_effect_data);
            break;

        case EFFECT_CYCLE:
            result = (Effect*)new CycleEffect(current_cycle_effect_data.colors, current_cycle_effect_data.color_duration);
            break;

        case EFFECT_WAVE:
            result = (Effect*)new WaveEffect(current_wave_effect_data);
            break;

        case EFFECT_CIRCULAR_WAVE:
            result = (Effect*)new CircularWaveEffect(current_circular_wave_effect_data);
            break;
    }
    
    current_command = EMPTY_COMMAND;
    effect = EMPTY_EFFECT;
    current_byte = 0;
    state = 0;

    return result;
}