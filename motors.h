#include <stdio.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN_PWM 0
#define MAX_PWM 4095

// PWM channels of PCA9685 0-16
#define PWM_CHANNEL1 12
#define PWM_CHANNEL2 13
#define PWM_CHANNEL3 10
#define PWM_CHANNEL4 11
#define PWM_CHANNEL5 8
#define PWM_CHANNEL6 9
#define PWM_CHANNEL7 14
#define PWM_CHANNEL8 15
#define INVERT_PULSE true

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo inputs
void setServo (uint8_t channel, uint16_t pulse) {
  pwm.setPWM(channel, 0, pulse);
}

// Drivetrain inputs
void setPWMMotors (int c1, int c2, int c3, int c4) {
  char dbg_str[30];
  sprintf(dbg_str,"C1: %d\tC2: %d\tC3: %d\tC4: %d",c1,c2,c3,c4);
  Serial.println(dbg_str);

  pwm.setPin(PWM_CHANNEL1, c1, INVERT_PULSE);
  pwm.setPin(PWM_CHANNEL2, c2, INVERT_PULSE);
  pwm.setPin(PWM_CHANNEL3, c3, INVERT_PULSE);
  pwm.setPin(PWM_CHANNEL4, c4, INVERT_PULSE);
}

// Other motors inputs
void setPWMMotors3 (int c1, int c2) 
{
  pwm.setPin(PWM_CHANNEL5, c1, INVERT_PULSE);
  pwm.setPin(PWM_CHANNEL6, c2, INVERT_PULSE);
}

void setPWMMotors4 (int c1, int c2) 
{
  pwm.setPin(PWM_CHANNEL7, c1, INVERT_PULSE);
  pwm.setPin(PWM_CHANNEL8, c2, INVERT_PULSE);
}
// Setting up
void initMotors() {
  Wire.begin(); // SDA, SCL, 400000);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Wire.setClock(400000);
  setPWMMotors3(0,0);
  setPWMMotors4(0,0);
  setPWMMotors(0, 0, 0, 0); }
