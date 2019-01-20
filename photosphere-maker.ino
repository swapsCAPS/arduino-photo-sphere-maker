#include <AccelStepper.h>
#include <Servo.h>

AccelStepper pan(AccelStepper::HALF4WIRE, 8, 10, 9, 11);

#define stepsPerRev   4096
#define outerTeeth    107
#define innerTeeth    15

float ratio;
float stepsPerTTRev;

void setup() {
  Serial.begin(9600);
  ratio         = outerTeeth / innerTeeth;
  stepsPerTTRev = stepsPerRev * ratio;
  pan.setCurrentPosition(0);
  pan.setMaxSpeed(1400.0);
  pan.setAcceleration(1000.0);
  Serial.println("enter degrees °");
}

int currentPos = 0;
int val;
int req;

void loop() {
  while (Serial.available() > 0) { // Something is in the serial buffer
    val = Serial.parseInt();
    currentPos = val;

    val *= -1;

    req = map(val, 0, 359, 0, stepsPerTTRev);

    String toPrint = "Moving ";
    toPrint += val;
    toPrint += "°";
    if (val > 0) {
      toPrint += " ccw";
    } else {
      toPrint += " cw";
    }

    Serial.println(toPrint);
    pan.move(req);
  }
  pan.run();
}
