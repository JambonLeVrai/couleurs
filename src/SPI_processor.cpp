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

bool SPIProcessor::setNewEffect(uint8_t byte) {
    if (effect == EMPTY_EFFECT) {
        effect = byte;
        current_byte = 0;
        return false;
    }

    switch(effect) {
        case EFFECT_FIXED:
            return processFixedEffect(byte);
            break;
    }
}

bool SPIProcessor::processFixedEffect(uint8_t byte)  {
    switch(current_byte) {
        case 0:
            current_fixed_effect_data.R = byte;
            current_byte++;
            break;
        case 1:
            current_fixed_effect_data.G = byte;
            current_byte++;
            break;
        case 2:
            current_fixed_effect_data.B = byte;
            return true;
            break;
    }

    return false;
}

Effect* SPIProcessor::getEffect() {
    Effect* result;
    switch(effect) {
        case EFFECT_FIXED:
            result = (Effect*)new FixedEffect(current_fixed_effect_data.R, current_fixed_effect_data.G, current_fixed_effect_data.B );
            break;
    }
    
    current_command = EMPTY_COMMAND;
    effect = EMPTY_EFFECT;
    current_byte = 0;
    state = 0;

    return result;
}