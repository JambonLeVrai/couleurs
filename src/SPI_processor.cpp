#include "SPI_processor.h"

SPIProcessor::SPIProcessor() {
    current_command = EMPTY_COMMAND;
    effect = EMPTY_EFFECT;
    current_byte = 0;
    state = 0;
}

ProcessedType SPIProcessor::receiveData(uint8_t byte) {
    if (current_command == EMPTY_COMMAND) {
        current_command = byte;
        return ProcessedType::NONE;
    }

    switch(current_command) {
        case COMMAND_SET_NEW_EFFECT:
            if(setNewEffect(byte))
                return ProcessedType::NEW_EFFECT;
            break;
        case COMMAND_KEY_PRESSED:
            if(setKeyPressed(byte))
                return ProcessedType::KEY_PRESSED;
            break;
        case COMMAND_KEY_RELEASED:
            if(setKeyReleased(byte))
                return ProcessedType::KEY_RELEASED;
            break;
    }

    return ProcessedType::NONE;
}

bool SPIProcessor::setKeyPressed(uint8_t byte) {
    pressed_key_code = byte;
    return true;
}

bool SPIProcessor::setKeyReleased(uint8_t byte) {
    released_key_code = byte;
    return true;
}

uint8_t SPIProcessor::keyPressed() {
    current_command = EMPTY_COMMAND;
    return pressed_key_code;
}

uint8_t SPIProcessor::keyReleased() {
    current_command = EMPTY_COMMAND;
    return released_key_code;
}

bool SPIProcessor::processCycleEffect(uint8_t byte, uint8_t& current) {
    if(current == 0) {
        current_cycle_effect_data.nb_colors = byte;
        current_cycle_effect_data.colors = new std::vector<ColorF>();
        current++;
        return false;
    }
    if(current <= 4 && current >= 1) {
        *((uint8_t*)&current_cycle_effect_data.color_duration + (current - 1)) = byte;
        current++;
        return false;
    }

    if((current-5) % 3 == 0) {
        current_cycle_effect_data.cur_r = byte;
        current++;
        return false;
    }
    else if((current-5) % 3 == 1) {
        current_cycle_effect_data.cur_g = byte;
        current++;
        return false;
    }
    else {
        current_cycle_effect_data.cur_b = byte;
        current++;
        current_cycle_effect_data.colors->push_back(color_from_rgb(current_cycle_effect_data.cur_r, current_cycle_effect_data.cur_g, current_cycle_effect_data.cur_b));

        return (current-5) / 3 > current_cycle_effect_data.nb_colors-1;
    }
}

bool SPIProcessor::processFixedEffect(uint8_t byte, uint8_t& current) {
    *((uint8_t*)(&current_fixed_effect_data) + current) = byte;
    current++;
    return current == sizeof(FixedEffectData);
}

bool SPIProcessor::processWaveEffect(uint8_t byte, uint8_t& current) {
    *((uint8_t*)(&current_wave_effect_data) + current) = byte;
    current++;
    return (current == sizeof(WaveEffectData));
}

bool SPIProcessor::processCircularWaveEffect(uint8_t byte, uint8_t& current) {
    *((uint8_t*)(&current_circular_wave_effect_data) + current) = byte;
    current++;
    return (current == sizeof(CircularWaveEffectData));
}

bool SPIProcessor::processFixedPatternEffect(uint8_t byte, uint8_t& current) {
    if(current == 0)
        current_fixed_pattern_effect_data.clear();

    switch(current % 3) {
        case 0:
            current_fixed_pattern_effect_data.push_back(ColorI());
            current_fixed_pattern_effect_data.back().R = byte;
            break;
        case 1:
            current_fixed_pattern_effect_data.back().G = byte;
            break;
        case 2:
            current_fixed_pattern_effect_data.back().B = byte;
            break;
    }
    current++;
    Serial.println(current);
    return (current == KEYS*3);
}


bool SPIProcessor::processPersistenceEffect(uint8_t byte, uint8_t& current) {
    *((uint8_t*)(&current_persistence_effect_data) + current) = byte;
    current++;
    return (current == sizeof(PersistenceEffectData));
}


bool SPIProcessor::processCompoundEffect(uint8_t byte) {
    if(current_byte == 0) {
        current_compound_effect_data.nb_effects = byte;
        current_compound_effect_data.current_effect_type = EMPTY_EFFECT;
        current_compound_effect_data.current_effect = 0;
        current_byte++;
        return false;
    }

    switch(current_compound_effect_data.current_effect_type) {
        case EMPTY_EFFECT:
            current_compound_effect_data.current_effect_type = byte;
            current_compound_effect_data.local_byte = 0;
            return false;
            break;
        case EFFECT_FIXED:
            if(processFixedEffect(byte, current_compound_effect_data.local_byte)) {
                current_compound_effect_data.effects.push_back(getRawEffect(current_compound_effect_data.current_effect_type));
                current_compound_effect_data.current_effect_type = EMPTY_EFFECT;
                return ++current_compound_effect_data.current_effect == current_compound_effect_data.nb_effects;
            }
            break;
        case EFFECT_CYCLE:
            if(processCycleEffect(byte, current_compound_effect_data.local_byte)) {
                current_compound_effect_data.effects.push_back(getRawEffect(current_compound_effect_data.current_effect_type));
                current_compound_effect_data.current_effect_type = EMPTY_EFFECT;
                return ++current_compound_effect_data.current_effect == current_compound_effect_data.nb_effects;
            }
            break;
        case EFFECT_WAVE:
            if(processWaveEffect(byte, current_compound_effect_data.local_byte)) {
                current_compound_effect_data.effects.push_back(getRawEffect(current_compound_effect_data.current_effect_type));
                current_compound_effect_data.current_effect_type = EMPTY_EFFECT;
                return ++current_compound_effect_data.current_effect == current_compound_effect_data.nb_effects;
            }
            break;
        case EFFECT_CIRCULAR_WAVE:
            if(processCircularWaveEffect(byte, current_compound_effect_data.local_byte)) {
                current_compound_effect_data.effects.push_back(getRawEffect(current_compound_effect_data.current_effect_type));
                current_compound_effect_data.current_effect_type = EMPTY_EFFECT;
                return ++current_compound_effect_data.current_effect == current_compound_effect_data.nb_effects;
            }
            break;
        case EFFECT_FIXED_PATTERN:
            if(processFixedPatternEffect(byte, current_compound_effect_data.local_byte)) {
                current_compound_effect_data.effects.push_back(getRawEffect(current_compound_effect_data.current_effect_type));
                current_compound_effect_data.current_effect_type = EMPTY_EFFECT;
                return ++current_compound_effect_data.current_effect == current_compound_effect_data.nb_effects;
            }
            break;
        case EFFECT_PERSISTENCE:
            if(processPersistenceEffect(byte, current_compound_effect_data.local_byte)) {
                current_compound_effect_data.effects.push_back(getRawEffect(current_compound_effect_data.current_effect_type));
                current_compound_effect_data.current_effect_type = EMPTY_EFFECT;
                return ++current_compound_effect_data.current_effect == current_compound_effect_data.nb_effects;
            }
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
            return processFixedEffect(byte, current_byte);
            break;

        case EFFECT_CYCLE:
            return processCycleEffect(byte, current_byte);
            break;
        
        case EFFECT_WAVE:
            return processWaveEffect(byte, current_byte);
            break;

        case EFFECT_CIRCULAR_WAVE:
            return processCircularWaveEffect(byte, current_byte);
            break;

        case EFFECT_COMPOUND:
            return processCompoundEffect(byte);
            break;

        case EFFECT_FIXED_PATTERN:
            return processFixedPatternEffect(byte, current_byte);
            break;

        case EFFECT_PERSISTENCE:
            return processPersistenceEffect(byte, current_byte);
            break;
    }

}


Effect* SPIProcessor::getRawEffect(uint8_t effect_type) {
    Effect* result;
    switch(effect_type) {
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

        case EFFECT_COMPOUND:
            result = (Effect*)new CompoundEffect(current_compound_effect_data.effects);
            current_compound_effect_data.effects.clear();
            break;

        case EFFECT_FIXED_PATTERN:
            result = (Effect*)new FixedPatternEffect(current_fixed_pattern_effect_data);
            break;

        case EFFECT_PERSISTENCE:
            result = (Effect*)new PersistenceEffect(current_persistence_effect_data);
            break;
    }
    return result;
}

Effect* SPIProcessor::getEffect() {
    Effect* result = getRawEffect(effect);
    
    current_command = EMPTY_COMMAND;
    effect = EMPTY_EFFECT;
    current_byte = 0;
    state = 0;

    return result;
}