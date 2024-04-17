#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "variables.h"

void initialize();
void processCommands();
void moveGantry(float targetX, float targetY);
void toggleServo(int interval);
void moveGantryToHome();

#endif
