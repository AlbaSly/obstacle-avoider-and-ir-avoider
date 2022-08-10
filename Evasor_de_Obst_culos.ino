#include <Servo.h>
#include <NewPing.h>

Servo servoMotor;

int SERVO_DELAY = 800;

// Definitions Arduino pins connected to input H Bridge
int IN1 = 8;
int IN2 = 9;
int IN3 = 11;
int IN4 = 12;

int pinTrigger = 3; //Trigger
int pinEcho = 4; //Echo

int leftIR = 6;
int rightIR = 7;

NewPing sonic(pinTrigger, pinEcho, 200);
 
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(leftIR, INPUT);
  pinMode(rightIR, INPUT);

  servoMotor.attach(2);

  Serial.begin(9600);
}

int distance = 0;
int distanceToRight = 0;
int distanceToLeft = 0;

void loop()
{
  distance = calcDistance();

  if (distance >= 20 && distance <= 250) {
    runForward();

    if (digitalRead(leftIR) && digitalRead(rightIR)) {
      stopMotors();
      delay(1000);
      runBackward();
      delay(600);
      runToLeft();
      delay(800);
    }
  }
  else if (distance < 20) {
    stopMotors();
    delay(150);
    runBackward();
    delay(250);
    stopMotors();
    delay(250);
    distanceToRight = lookToRight();
    delay(250);
    distanceToLeft = lookToLeft();
    delay(400);

    if (distanceToRight >= distanceToLeft) {
      runToRight();
      delay(400);
      stopMotors();
      delay(500);
    }
    else if (distanceToLeft >= distanceToRight) {
      runToLeft();
      delay(400);
      stopMotors();
      delay(500);
    }
  }
}

int calcDistance() {
  delay(5);

  int distance = sonic.ping_cm();

  if (distance <= 0 || distance >= 250) {
    distance = 250;
  }

  return distance;
}

int lookToLeft() {
  servoMotor.write(180);
  delay(SERVO_DELAY/2);
  int distance = calcDistance();
  delay(SERVO_DELAY/2);
  servoMotor.write(90);

  return distance;
}

int lookToRight() {
  servoMotor.write(0);
  delay(SERVO_DELAY/2);
  int distance = calcDistance();
  delay(SERVO_DELAY/2);
  servoMotor.write(90);

  return distance;
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void runForward() {

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void runBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void runToLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void runToRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
