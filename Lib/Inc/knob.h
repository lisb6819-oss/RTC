#ifndef KNOB_H
#define KNOB_H

// Knob-related declarations go here
#include "time.h"

typedef void (*KnobCallback)(void);

void Knob_Init();
void Knob_Loop();
void Knob_SetForwardCallback(KnobCallback callback);
void Knob_SetBackwardCallback(KnobCallback callback);
void Knob_SetPressCallback(KnobCallback callback);

#endif // KNOB_H