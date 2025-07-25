#include <PS2X_lib.h>

#include <PS2X_lib.h>

PS2X ps2x; // create PS2 Controller Class object
#define BEBUG_CTRL

// Function prototypes
void setPWMMotors(int c1, int c2, int c3, int c4);
void setServo(uint8_t channel, uint16_t pulse);
void setPWMMotors3(int c1, int c2);
void setPWMMotors4(int c1, int c2);
// calibration for different kinds of PS2 controller
#define X_JOY_CALIB 127
#define Y_JOY_CALIB 128

#define PS2_DAT 12 // MISO  19
#define PS2_CMD 13 // MOSI  23
#define PS2_SEL 15 // SS     5
#define PS2_CLK 14 // SLK   18
// When replacing controller reciever, MUST REPLACE 3.3V AND GND PIN (take the cable out, replace em)
#define NORM_SPEED 2100 // Robot's speed, you can add a boolean to change Robot's speed by pressing a button
#define TURNING_FACTOR 1

void setupPS2controller() {
    int err = -1;
    while (err != 0) {
        err = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
    }
}
// Optimized for Arcade driving style (2 Joysticks)
bool PS2control() {
    int speed = NORM_SPEED;
    int nJoyY = X_JOY_CALIB - ps2x.Analog(PSS_RX); // read x-joystick
    int nJoyX = Y_JOY_CALIB - ps2x.Analog(PSS_LY); // read y-joystick
    int nMotMixL; // Motor (left) mixed output
    int nMotMixR; // Motor (right) mixed output

    if (nJoyX == -1 && nJoyY == 0) { // in case of lost connection
        setPWMMotors(0, 0, 0, 0);
        return 0;
    }

    // Change the "+" with "-" and the opposite or Single Joystick Driving style
    nMotMixL = nJoyY + nJoyX;
    nMotMixR = nJoyY - nJoyX;

    #ifdef BEBUG_CTRL
    Serial.print(F("Left motor mix: "));
    Serial.print(nMotMixL);
    Serial.print(F(" | Right motor mix: "));
    Serial.println(nMotMixR);
    #endif

    int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

    if (nMotMixR > 10) {
        c3 = nMotMixR;
        c3 = map(c3, 0, 128, 0, speed);
    } else if (nMotMixR < (-10)) {
        c4 = abs(nMotMixR);
        c4 = map(c4, 0, 128, 0, speed);
    }

    if (nMotMixL > 10) {
        c1 = nMotMixL;
        c1 = map(c1, 0, 128, 0, speed);
    } else if (nMotMixL < (-10)) {
        c2 = abs(nMotMixL);
        c2 = map(c2, 0, 128, 0, speed);
    }

    setPWMMotors(c4, c3, c2, c1);
// Change the ports at 'motors.h'

// Stage 2 Lift, port 3 & 6
    if (ps2x.Button(PSB_L1)) {
        Serial.println("L1 pressed");
        setServo(6, 450);
        setServo(3, 450);
    } else if (ps2x.Button(PSB_R1)) {
        Serial.println("R1 pressed");
        setServo(6, 100);
        setServo(3, 100);
    } else {
        setServo(6, 307);
        setServo(3, 307);
    }

// Stage 2 Lift, Motor 1&4, 8.9.14.15
    if (ps2x.Button(PSB_L2)) {
        Serial.println("L2 pressed");
        setPWMMotors4(0, 3000);
        setPWMMotors3(3000, 0);
    } else if (ps2x.Button(PSB_R2)) {
        Serial.println("R2 pressed");
        setPWMMotors4(3000, 0);
        setPWMMotors3(0, 3000);
    } else {
        setPWMMotors3(0, 0);
        setPWMMotors4(0, 0);
    }


// Hold arm, port 4

static bool servoState1 = false;
static bool lastButtonState1 = false;
bool currentButtonState1 = ps2x.Button(PSB_GREEN);

if (currentButtonState1 && !lastButtonState1) {
  servoState1 = !servoState1;
  if (servoState1) {
    setServo(4, 307); // servo at port 4, 0 degree state
    Serial.println("Green/Triangle pressed");
  } else {
    setServo(4, 450); // servo at port 4, 180 degree state
    Serial.println("Green/Triangle pressed");
  }
}
lastButtonState1 = currentButtonState1;
// If 'Green' was pressed, change to OPEN position. If it's pressed again, change to CLOSED position

// Fruit holder, port 5
    if (ps2x.Button(PSB_CIRCLE)) {
        Serial.println("SQUARE/PURPLE pressed");
        setServo(5, 100);
    } else if (ps2x.Button(PSB_SQUARE)) {
        Serial.println("CIRCLE/RED pressed");
        setServo(5, 410);
    } else {
        setServo(5, 307);
    }
    return 1;
}
