#include <Stepper.h>
#include <Servo.h>

Stepper pan(2048, 8, 10, 9, 11);

Servo servoTilt;

void setup() {
  Serial.begin(9600);
  servoTilt.attach(7);
}

void loop() {
  int panVal  = analogRead(A0);
  int tiltVal = analogRead(A1);

  int speed = map(panVal, 0, 1023, 0, 16);
  int tilt  = map(tiltVal, 0, 1023, 0, 180);

  Serial.println(tilt);

  servoTilt.write(tilt);

  if (speed > 0) {
    pan.setSpeed(speed);
    pan.step(2048/ 100);
  }
}
