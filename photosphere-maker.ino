#include <AccelStepper.h>
#include <Servo.h>

#define stepsPerRev  4096
#define outerTeeth   107
#define innerTeeth   15
#define servoOffset  40

AccelStepper pan(AccelStepper::HALF4WIRE, 8, 10, 9, 11);

Servo servoL;
Servo servoR;

float ratio;
float stepsPerTTRev;

void setup() {
  Serial.begin(9600);
  servoL.attach(6);
  servoR.attach(7);
  ratio         = outerTeeth / innerTeeth;
  stepsPerTTRev = stepsPerRev * ratio;

  pan.setCurrentPosition(0);
  pan.setMaxSpeed(1400.0);
  pan.setAcceleration(1000.0);

  Serial.println("enter degrees °");
}

int val;
int req;
String str;
String type;
int indexOf;

void loop() {
  while (Serial.available() > 0) { // Something is in the serial buffer
    str  = Serial.readString();
    indexOf = str.indexOf(" ");
    type    = str.substring(0, indexOf);
    val     = str.substring(indexOf).toInt();

    Serial.println(type);
    Serial.println(val);

    if (type == "pan") {
      return // Dont do anything for now
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
      return;
    }

    if (type = "tilt") {
      if (val < 0 || val > 180) {
        Serial.println("Please enter: (val >= 0 || val <= 180)");
        return;
      }
      servoL.write(abs(val - 180));
      servoR.write(val);

      return;
    }

  }
  pan.run();
}
