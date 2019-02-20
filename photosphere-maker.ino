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
  pan.setAcceleration(100.0);

  servoL.attach(6);
  servoR.attach(7);
  servoL.write(92);
  servoR.write(92);
  delay(2000);
  servoL.detach();
  servoR.detach();
}

bool started = false;

  // Degrees:
  // 120 *  3 @ 25
  // 40  *  9 @ 55
  // 30  * 12 @ 90
  // 40  *  9 @ 125 ?
  // 120 *  3 @ 155 ?
float degrees[]    = { 30.0, 40.0, 120.0,  40.0, 120.0 };
float offsets[]    = {  0,  -15.0,   0,     0,     0   };
int amounts[]      = { 1,    1,     1,     1,     1   };
/* int amounts[]      = { 11,    9,     3,     9,     3   }; */
float tiltAngles[] = { 92,   57,    23,   130,   155   };

int step   = 0;
int amount = 0;

bool offsetting = false;

void loop() {
  pan.run();
  while (Serial.available() > 0) { // Something is in the serial buffer
    String str  = Serial.readString();

    if (str == "stop") {
      started = false;
      pan.stop();
    }

    if (str == "start") {
      started = true;

      servoL.attach(6);
      servoR.attach(7);
      servoL.write(abs(tiltAngles[step] - 180));
      servoR.write(tiltAngles[step]);

      float req = (stepsPerTTRev / 360.0) * degrees[amount];
      Serial.println(amount);
      pan.move(req);
    }
  }

  if (started == true) {

    if (offsetting == true) {
      if (abs(pan.distanceToGo()) > 0) { return; } // Still need to move
      offsetting = false;                          // No more movement, stop offsetting
      return;
    }

    if (amount < amounts[step]) {
      if (abs(pan.distanceToGo()) > 0) { return; } // Still need to move

      // Movement done do next move
      amount++;
      float req = (stepsPerTTRev / 360.0) * degrees[step];
      delay(500);
      pan.move(req);

    } else {
      step++; // Reached end of the steps on this level go to next

      if (step > 4) { // Reached the end
        started = false;
        pan.stop();
        servoL.attach(6);
        servoR.attach(7);
        servoL.write(abs(92 - 180));
        servoR.write(92);

        return Serial.println("Done");
      }

      amount = 0;
      servoL.attach(6);
      servoR.attach(7);
      servoL.write(abs(tiltAngles[step] - 180));
      servoR.write(tiltAngles[step]);

      delay(1000);

      // Do offset move if we need it
      offsetting = true;
      float req = (stepsPerTTRev / 360.0) * offsets[step];
      pan.move(req);
    }
  }
}
