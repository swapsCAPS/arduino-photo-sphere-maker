#include <AccelStepper.h>
#include <Servo.h>

#define stepsPerRev  2048.0 // 4096
#define outerTeeth   107.0
#define innerTeeth   15.0
#define servoOffset  40

AccelStepper pan(AccelStepper::FULL4WIRE, 8, 10, 9, 11);

Servo servoL;
Servo servoR;

struct Step {
  float deg;
  float tiltAngle;
};

float stepsPerTTRev;

int step     = 0;
bool started = false;

void setup() {
  Serial.begin(9600);
  float ratio = outerTeeth / innerTeeth;
  stepsPerTTRev = stepsPerRev * ratio;

  pan.setCurrentPosition(0);
  pan.setMaxSpeed(350.0);
  pan.setAcceleration(100.0);

  servoL.write(90);
  servoR.write(90);
  delay(2000);
  servoL.detach();
  servoR.detach();
}

// Yes, yes. We could create a complicated loop to step through the steps,
// but this is way more understandable.
#define TOTAL_STEPS 33
Step steps[] = {
  { 30.0, 90 },
  { 30.0, 90 },
  { 30.0, 90 },
  { 30.0, 90 },
  { 30.0, 90 },
  { 30.0, 90 },
  { 30.0, 90 },
  { 30.0, 90 },
  { 30.0, 90 },
  { 30.0, 90 },
  { 30.0, 90 },
  { 30.0, 90 },
  { -10.0, 55 }, // Offset slightly to align with the 'dot'
  { 40.0, 55 },
  { 40.0, 55 },
  { 40.0, 55 },
  { 40.0, 55 },
  { 40.0, 55 },
  { 40.0, 55 },
  { 40.0, 55 },
  { 40.0, 55 },
  { 40.0, 55 },
  { 40.0, 125 },
  { 40.0, 125 },
  { 40.0, 125 },
  { 40.0, 125 },
  { 40.0, 125 },
  { 40.0, 125 },
  { 40.0, 125 },
  { 40.0, 125 },
  { 40.0, 125 },
  { 360.0, 23 },
  { 360.0, 155 }
};

void Tilt(int angle) {
  Serial.print("Tilt: ");
  Serial.println(angle);
  servoL.write(abs(angle - 180));
  servoR.write(angle);
}

void Pan(float angle) {
  Serial.print("Pan:  ");
  Serial.println(angle);
  float req = (stepsPerTTRev / 360.0) * angle;
  pan.move(req);
}

void Stop() {
  started = false;
  step    = 0;
  pan.stop();
  Tilt(steps[step].tiltAngle);
  delay(1500);
  servoL.detach();
  servoR.detach();
  Serial.println("Stopped");
}

void loop() {
  pan.run();
  while (Serial.available() > 0) { // Something is in the serial buffer
    String str  = Serial.readString();

    if (str == "stop") {
      Stop();
    }

    if (str == "start") {
      started = true;
      servoL.attach(6);
      servoR.attach(7);
      Tilt(steps[step].tiltAngle);
      Pan(steps[step].deg);
    }
  }

  if (started == true) {
    if (abs(pan.distanceToGo()) > 0) { return; } // Still need to move

    delay(200);

    step++;                                      // Movement done do next move

    if (step > TOTAL_STEPS) { return Stop(); }   // Reached the end

    Serial.print("Step: ");
    Serial.print(step);
    Serial.print(" - ");
    Serial.println(TOTAL_STEPS);

    Tilt(steps[step].tiltAngle);
    Pan(steps[step].deg);

    Serial.println("");
  }
}
