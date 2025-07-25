#include <PS2_controller.h>
#include <motors.h>
#include <Arduino.h>

void setup() {
    Serial.begin(115200); // Start serial communication for debugging
    initMotors();         // Initialize motors
    setupPS2controller(); // Set up the PS2 controller
}

void loop() {
    ps2x.read_gamepad(); // Call the function to read the gamepad input
    PS2control(); // Call the function to control motors based on PS2 input
    delay(50); // Small delay to avoid overwhelming the loop
}