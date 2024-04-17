#include "functions.h"

AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY1(AccelStepper::DRIVER, Y1_STEP_PIN, Y1_DIR_PIN);
AccelStepper stepperY2(AccelStepper::DRIVER, Y2_STEP_PIN, Y2_DIR_PIN);
Servo servoXs;

float homeX = 0;
float homeY = 0;

void initialize() {
  stepperX.setMaxSpeed(MAX_SPEED_X);
  stepperX.setAcceleration(ACCELERATION_X);
  stepperY1.setMaxSpeed(MAX_SPEED_Y);
  stepperY1.setAcceleration(ACCELERATION_Y);
  stepperY2.setMaxSpeed(MAX_SPEED_Y);
  stepperY2.setAcceleration(ACCELERATION_Y);

  servoX.attach(X_SERVO_PIN);
  servoX.write(177);

  Serial.begin(115200);

  pinMode(X_LIMIT_SWITCH_PIN, INPUT_PULLUP);
  pinMode(Y1_LIMIT_SWITCH_PIN, INPUT_PULLUP);
  pinMode(Y2_LIMIT_SWITCH_PIN, INPUT_PULLUP);

  moveGantryToHome();
}

void processCommands() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input == "R") {
      moveGantryToHome();
      Serial.println("OK");
    } else {
      int commaIndex1 = input.indexOf(',');
      int commaIndex2 = input.indexOf(',', commaIndex1 + 1);
      if (commaIndex1 != -1 && commaIndex2 != -1) {
        float xTarget = input.substring(0, commaIndex1).toFloat();
        float yTarget = input.substring(commaIndex1 + 1, commaIndex2).toFloat();
        int interval = input.substring(commaIndex2 + 1).toInt();
        if (xTarget <= X_MAX && yTarget <= Y_MAX) {
          moveGantry(xTarget, yTarget);
          if (interval != 0) {
            toggleServo(interval);
          }
          Serial.println("OK");
        } else {
          Serial.println("OK");
          Serial.println("Out of Range");
        }
      }
    }
  }
}

void moveGantry(float targetX, float targetY) {
  long int targetPositionX = targetX * STEPS_PER_MM;
  long int targetPositionY1 = targetY * STEPS_PER_MM;
  long int targetPositionY2 = targetY * STEPS_PER_MM;

  stepperX.moveTo(targetPositionX);
  stepperY1.moveTo(targetPositionY1);
  stepperY2.moveTo(targetPositionY2);

  bool x = true;
  bool y1 = true;
  bool y2 = true;

  while (x || y1 || y2) {
    x = stepperX.run();
    y1 = stepperY1.run();
    y2 = stepperY2.run();
  }
}

void toggleServo(int interval) {
  int angle_on = 155;
  int angle_off = 177;

  servoX.write(angle_on);
  delay(interval * 1000);
  servoX.write(angle_off);
}

void moveGantryToHome() {
  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(2000);
  while (digitalRead(X_LIMIT_SWITCH_PIN) == HIGH) {
    stepperX.moveTo(-5550);
    stepperX.run();
    delay(1);
  }
  stepperX.setCurrentPosition(0);
  stepperX.setMaxSpeed(MAX_SPEED_X);
  stepperX.setAcceleration(ACCELERATION_X);

  stepperY1.setMaxSpeed(1000);
  stepperY1.setAcceleration(2000);
  stepperY2.setMaxSpeed(1000);
  stepperY2.setAcceleration(2000);
  while (digitalRead(Y1_LIMIT_SWITCH_PIN) == HIGH || digitalRead(Y2_LIMIT_SWITCH_PIN) == HIGH) {
    stepperY1.moveTo(-2475);
    stepperY2.moveTo(-2475);
    if (digitalRead(Y1_LIMIT_SWITCH_PIN) == HIGH) {
      stepperY1.run();
    }
    if (digitalRead(Y2_LIMIT_SWITCH_PIN) == HIGH) {
      stepperY2.run();
    }
  }
  stepperY1.setCurrentPosition(0);
  stepperY2.setCurrentPosition(0);
  stepperY1.setMaxSpeed(MAX_SPEED_Y);
  stepperY1.setAcceleration(ACCELERATION_Y);
  stepperY2.setMaxSpeed(MAX_SPEED_Y);
  stepperY2.setAcceleration(ACCELERATION_Y);

  homeX = 0;
  homeY = 0;
}
