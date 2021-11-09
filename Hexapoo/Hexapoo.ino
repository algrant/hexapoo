// MultiStepper.pde
// -*- mode: C++ -*-
// Use MultiStepper class to manage multiple steppers and make them all move to 
// the same position at the same time for linear 2d (or 3d) motion.
#define HALFSTEP 8  

#include <AccelStepper.h>
#include <MultiStepper.h>

// EG X-Y position bed driven by 2 steppers
// Alas its not possible to build an array of these with different pins for each :-(

AccelStepper stepper1(AccelStepper::HALF4WIRE, 2, 4, 3, 5);
AccelStepper stepper2(AccelStepper::HALF4WIRE, 37, 39, 38, 40);
AccelStepper stepper3(AccelStepper::HALF4WIRE, 6, 8, 7, 9);
AccelStepper stepper4(AccelStepper::HALF4WIRE, 25, 27, 26, 28);
AccelStepper stepper5(AccelStepper::HALF4WIRE, 29, 31, 30, 32);
AccelStepper stepper6(AccelStepper::HALF4WIRE, 33, 35, 34, 36);

AccelStepper allSteppers[] = {
  stepper1,
  stepper2,
  stepper3,
  stepper4,
  stepper5,
  stepper6
};

// endstops -- p14 -> 19
int endPin1 = 16;
int endPin2 = 19;
int endPin3 = 14;
int endPin4 = 15;
int endPin5 = 18;
int endPin6 = 17;
int endPins[] = {
  endPin1,
  endPin2,
  endPin3,
  endPin4,
  endPin5,
  endPin6,
};

// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper StepperController;
// each stepper will have a state
// state[0] => "HOMING"

int bot_state = 0;
int stepper_states[] = { 0, 0, 0, 0, 0, 0 };
long int stepper_pos[] = { 0, 0, 0, 0, 0, 0 };

long int fun_pos[] = { 1600, -1600, 1600, -1600, 1600, -1600 };
long int no_pos[] = { 50, -50, 50, -50, 50, -50 };

void setup() {
  Serial.begin(9600);

  // Configure each stepper & pin
  int speed = 100;
  for (int i = 0; i < 6; i++) {
    allSteppers[i].setMaxSpeed(speed);
    StepperController.addStepper(allSteppers[i]);
    pinMode(endPins[i], INPUT_PULLUP);
  }
}



void loop() {
  int still_homing = false;

  for (int i = 0; i < 6; i++) {
    // update StepperController...
    int state = stepper_states[i];

    if (state == 0) {
      // homing
      int home_move = i%2 == 0 ? -1 : 1;
      if (digitalRead(endPins[i])) {
        // haven't yet hit end stop
        stepper_pos[i] += home_move;
        still_homing = true;
      } else {
        stepper_states[i] = 1; // ready
        stepper_pos[i] = 0;
        allSteppers[i].setCurrentPosition(0);
      }
    }

    // go up and down over time...
    if (state == 1 && bot_state == 1) {
      stepper_pos[i] = fun_pos[i];
    }

    // go up and down over time...
    if (state == 1 && bot_state == 2) {
      stepper_pos[i] = no_pos[i];
    }
  }

  // update bot state
  
  if (bot_state == 0) {
    if (!still_homing){
      bot_state = 1;
      int speed = 500;
      for (int i = 0; i < 6; i++) {
        allSteppers[i].setMaxSpeed(speed);
      }
      delay(1000);
    }
  } else if (bot_state == 1) {
    bot_state = 2;
  } else if (bot_state == 2) {
    bot_state = 1;
  }

  StepperController.moveTo(stepper_pos);
  StepperController.runSpeedToPosition();
}

void printEndStopState() {
  String buttonStates = "";
  for ( int i = 0; i < 6; i++ ) {
    if (digitalRead(endPins[i])) {
      buttonStates += "1";
    } else {
      buttonStates += "0";
    }
  }

  Serial.println(buttonStates);
}

void updatePositions() {
  long positions[6]; // Array of desired stepper positions

  positions[0] = 0;
  positions[1] = 0;
  positions[2] = 0;
  positions[3] = 0;
  positions[4] = 0;
  positions[5] = 0;
  StepperController.moveTo(positions);
  StepperController.runSpeedToPosition(); // Blocks until all are in position
  delay(2000);
  
  // Move to a different coordinate
  positions[0] =   100;
  positions[1] =  -100;
  positions[2] =   100;
  positions[3] =  -100;
  positions[4] =   100;
  positions[5] =  -100;
  
  StepperController.moveTo(positions);
  StepperController.runSpeedToPosition(); // Blocks until all are in position
  delay(2000);
}
