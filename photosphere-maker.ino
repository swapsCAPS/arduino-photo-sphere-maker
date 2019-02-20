#include <AccelStepper.h>
#include <Servo.h>

#define stepsPerRev  2048.0 // 4096
#define outerTeeth   107.0
#define innerTeeth   15.0
#define servoOffset  40

AccelStepper pan(AccelStepper::FULL4WIRE, 8, 10, 9, 11);

Servo servoL;
Servo servoR;

float ratio;
float stepsPerTTRev;

void setup() {
  Serial.begin(9600);
  ratio         = outerTeeth / innerTeeth;
  stepsPerTTRev = stepsPerRev * ratio;

  pan.setCurrentPosition(0);
  pan.setMaxSpeed(250.0);
  pan.setAcceleration(250.0);
  servoL.attach(6);
  servoR.attach(7);
  servoL.write(0);
  servoR.write(0);
  servoL.detach();
  servoR.detach();

}

bool started = false;

float degrees[] = { 30.0, 40.0, 120.0 };
/* int amounts[]   = { 12, 9, 3 }; */
int amounts[]   = { 1, 1, 1 };
float tiltAngles[] = { 90, 55, 25 };

int step   = 0;
int amount = 0;

void loop() {
  pan.run();
  while (Serial.available() > 0) { // Something is in the serial buffer
    String str  = Serial.readString();

    if (str == "start") {
      started = true;

      servoL.attach(6);
      servoR.attach(7);
      servoL.write(abs(tiltAngles[step] - 180));
      servoR.write(tiltAngles[step]);
      servoL.detach();
      servoR.detach();

      float req = (stepsPerTTRev / 360.0) * degrees[amount];
      Serial.println(amount);
      pan.move(req);
    }
  }

  if (started == true) {
    if (amount < amounts[step]) {
      if (pan.distanceToGo() <= 0) {
        amount++;
        Serial.println("status");
        Serial.println(step);
        Serial.println(amount);
        Serial.println("");
        float req = (stepsPerTTRev / 360.0) * degrees[step];
        delay(500);
        pan.move(req);
      }
    } else {
      step++;
      amount = 0;
      servoL.attach(6);
      servoR.attach(7);
      servoL.write(abs(tiltAngles[step] - 180));
      servoR.write(tiltAngles[step]);
      servoL.detach();
      servoR.detach();
    }
  }
  // Degrees:
  // 120 *  3 @ 25
  // 40  *  9 @ 55
  // 30  * 12 @ 90
  // 40  *  9 @ 125 ?
  // 120 *  3 @ 155 ?
}
