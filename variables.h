#ifndef VARIABLES_H
#define VARIABLES_H

#include <AccelStepper.h>
#include <Servo.h>

#define X_STEP_PIN 2
#define X_DIR_PIN 5
#define Y1_STEP_PIN 3
#define Y1_DIR_PIN 6
#define Y2_STEP_PIN 4
#define Y2_DIR_PIN 7

#define X_SERVO_PIN A3

#define X_LIMIT_SWITCH_PIN 9
#define Y1_LIMIT_SWITCH_PIN 10
#define Y2_LIMIT_SWITCH_PIN 11

#define STEPS_PER_MM 5
#define MAX_SPEED_X 1000
#define MAX_SPEED_Y 2000
#define ACCELERATION_X 2000
#define ACCELERATION_Y 2000

#define X_MAX 900
#define Y_MAX 400

extern AccelStepper stepperX;
extern AccelStepper stepperY1;
extern AccelStepper stepperY2;
extern Servo servoZ;

extern float homeX;
extern float homeY;

#endif
