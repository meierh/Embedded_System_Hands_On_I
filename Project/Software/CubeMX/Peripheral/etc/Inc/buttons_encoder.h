//
// Created by Tobias Henig on 23.09.24.
//

#ifndef BUTTONS_ENCODER_H
#define BUTTONS_ENCODER_H

/**
 * Minimum interval between two encoder events in milliseconds
 * This is used to debounce the encoder
 */
#define ROTARY_ENCODER_MIN_INTERVAL 20 // ms (note: SysTick interrupt priority is lower than interrupts for display and encoder)

#endif //BUTTONS_ENCODER_H
