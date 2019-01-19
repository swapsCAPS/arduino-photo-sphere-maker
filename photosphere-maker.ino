#include <AccelStepper.h>
#include <Servo.h>

AccelStepper pan(AccelStepper::FULL4WIRE, 8, 10, 9, 11);

void setup() {
  Serial.begin(9600);
  pan.setMaxSpeed(17.0);
  pan.setAcceleration(100.0);
  pan.setCurrentPosition(0);
}

int panPos;
void loop() {
  while (Serial.available() > 0) { // Something is in the serial buffer
    panPos = Serial.parseInt();
    String toPrint = "received ";
    toPrint += panPos;
    Serial.println(toPrint);
    delay(1000);
    Serial.println("moving");
    pan.moveTo(panPos);
    Serial.println("done");
  }
}
