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

  Serial.println("enter degrees °");
}

int deg;
float req;
String str;
String type;
int indexOf;

void loop() {
  pan.run();
  while (Serial.available() > 0) { // Something is in the serial buffer

    str  = Serial.readString();
    indexOf = str.indexOf(" ");
    type    = str.substring(0, indexOf);
    deg     = str.substring(indexOf).toFloat();

    Serial.println(type);
    Serial.println(deg);

    if (type == "pan") {
      servoL.detach();
      servoR.detach();
      String toPrint = "Moving ";
      toPrint += deg;
      toPrint += "°";
      if (deg > 0) {
        toPrint += " ccw";
      } else {
        toPrint += " cw";
      }

      Serial.println(toPrint);

      deg *= -1;

  // Degrees:
  // 120 *  3 @ 25
  // 40  *  9 @ 55
  // 30  * 12 @ 90
  // 40  *  9 @ 125 ?
  // 120 *  3 @ 155 ?

      req = (stepsPerTTRev / 360.0) * deg;

      Serial.println(req);
      pan.move(req);
      return;
    }

    if (type = "tilt") {
      servoL.attach(6);
      servoR.attach(7);
      if (deg < 0 || deg > 180) {
        Serial.println("Please enter: (deg >= 0 || deg <= 180)");
        return;
      }
      servoL.write(abs(deg - 180));
      servoR.write(deg);

      return;
    }

  }
}
