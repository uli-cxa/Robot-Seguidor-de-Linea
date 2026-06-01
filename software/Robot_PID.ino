#include <Arduino.h>
#include <QTRSensors.h>

const uint8_t sensorPins[] = {13, 12, 14, 26, 27, 25, 33, 32};
const uint8_t NUM_SENSORES = 8;

#define MOTOR_D_PWM  21
#define MOTOR_D_IN1  22
#define MOTOR_D_IN2  23
#define MOTOR_I_PWM  18
#define MOTOR_I_IN3  19
#define MOTOR_I_IN4   5

float Kp = 0.12f;
float Ki = 0.0f;
float Kd = 0.80f;

int VEL_RECTA = 200;
int VEL_CURVA = 80;

const int MAX_SPEED = 255;
const int MIN_SPEED = -70;
const int DEADBAND = 20;

QTRSensors qtr;
uint16_t sensores[8];
float lastError = 0;
float integral = 0;
bool calibrado = false;

void motorDerecho(int vel) {
  vel = constrain(vel, MIN_SPEED, MAX_SPEED);
  digitalWrite(MOTOR_D_IN1, vel > 0 ? HIGH : LOW);
  digitalWrite(MOTOR_D_IN2, vel < 0 ? HIGH : LOW);
  ledcWrite(MOTOR_D_PWM, abs(vel));
}

void motorIzquierdo(int vel) {
  vel = constrain(vel, MIN_SPEED, MAX_SPEED);
  digitalWrite(MOTOR_I_IN3, vel > 0 ? HIGH : LOW);
  digitalWrite(MOTOR_I_IN4, vel < 0 ? HIGH : LOW);
  ledcWrite(MOTOR_I_PWM, abs(vel));
}

void pararMotores() {
  motorIzquierdo(0);
  motorDerecho(0);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MOTOR_D_IN1, OUTPUT);
  pinMode(MOTOR_D_IN2, OUTPUT);
  pinMode(MOTOR_I_IN3, OUTPUT);
  pinMode(MOTOR_I_IN4, OUTPUT);

  ledcAttach(MOTOR_D_PWM, 5000, 8);
  ledcAttach(MOTOR_I_PWM, 5000, 8);
  pararMotores();

  qtr.setTypeAnalog();
  qtr.setSensorPins(sensorPins, NUM_SENSORES);
  qtr.setSamplesPerSensor(2);

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  delay(1000);

  digitalWrite(LED_BUILTIN, HIGH);
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
    delay(25);
  }
  digitalWrite(LED_BUILTIN, LOW);

  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(80);
    digitalWrite(LED_BUILTIN, LOW);
    delay(80);
  }
  delay(500);

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
  }

  calibrado = true;
}

void loop() {
  if (!calibrado) return;

  uint16_t position = qtr.readLineBlack(sensores);
  float error = (float)position - 3500.0f;
  if (abs(error) < DEADBAND) error = 0;

  float t = constrain(abs(error) / 2500.0f, 0.0f, 1.0f);
  int speedActual = (int)(VEL_RECTA * (1.0f - t) + VEL_CURVA * t);

  integral += error;
  integral = constrain(integral, -300.0f, 300.0f);
  float deriv = error - lastError;
  lastError = error;

  float correction = (Kp * error) + (Ki * integral) + (Kd * deriv);

  int leftSpeed = speedActual + (int)correction;
  int rightSpeed = speedActual - (int)correction;

  leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

  motorIzquierdo(leftSpeed);
  motorDerecho(rightSpeed);

  if (Serial.available()) {
    char c = Serial.read();
    float val = Serial.parseFloat();
    switch (c) {
      case 'p': case 'P': Kp = val; break;
      case 'i': case 'I': Ki = val; break;
      case 'd': case 'D': Kd = val; break;
      case 'r': case 'R': VEL_RECTA = constrain((int)val, 0, 255); break;
      case 'c': case 'C': VEL_CURVA = constrain((int)val, 0, 255); break;
      case '+': VEL_RECTA = min(VEL_RECTA + 10, 255); break;
      case '-': VEL_RECTA = max(VEL_RECTA - 10, 0); break;
    }
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
  }

  delay(3);
}
