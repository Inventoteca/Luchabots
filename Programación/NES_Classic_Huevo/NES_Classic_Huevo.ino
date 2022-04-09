/*
  Luchabot Huevo
*/

// Motor pins
#define motFrenteA A0
#define motAtrasA A1
#define motFrenteB A2
#define motAtrasB A3
#define PIN_SERVO_A 3
#define PIN_SERVO_B 6

// Controller library
#include <NintendoExtensionCtrl.h>
ClassicController classic;

// Servo library
#include <Servo.h>
Servo servoA;
Servo servoB;

// ?
bool autoStatus = false;

// Speed
int normalVel = 180; //Velocidad default, que sea menor a 255
int topVel = 255; //Velocidad máxima

void setup() {
  Serial.begin(115200);
  Serial.println("Piloto abordando Sumobot.");
  classic.begin();

  pinMode(motFrenteA, OUTPUT); //Pin con PWM
  pinMode(motAtrasA, OUTPUT); //Pin con PWM
  pinMode(motFrenteB, OUTPUT); //Pin con PWM
  pinMode(motAtrasB, OUTPUT); //Pin con PWM

  servoA.attach(PIN_SERVO_A);
  servoB.attach(PIN_SERVO_B);

  while (!classic.connect()) {
    Serial.println("Classic Controller not detected!");
    delay(1000);
  }
  Serial.println("Piloto listo. Arrancando Sumobot.");
}

void loop() { //////////////////////////////////////////////////////// LOOP

  boolean success = classic.update();  // Get new data from the controller

  if (!success) {  // Ruh roh
    Serial.println("Controller disconnected!");
    delay(1000);
  }
  else control();
}  //////////////////////////////////////////////////////// LOOP

void control() {
  // Read the DPAD (Up/Down/Left/Right) or read a button (ABXY, Minus, Plus)
  int vel = 255;
  boolean padUp = classic.dpadUp();
  boolean padDown = classic.dpadDown();
  boolean padRight = classic.dpadRight();
  boolean padLeft = classic.dpadLeft();

  boolean aButton = classic.buttonA();
  boolean bButton = classic.buttonB();

  if (!padUp && !padDown && !padRight && !padLeft) //No hay movimiento
  {
    Serial.println("-----------");
    motorOFF();
  }

  ////////////////
  if (bButton) //Bajar brazos
  {
    servoA.write(135);
    servoB.write(45);
  }
  if (aButton) //Alzar brazos
  {
    servoA.write(45);
    servoB.write(135);
  }
  ////////////////
  

  ////////////////

  if (padUp) //Para adelante
  {
    Serial.println("^");
    adelante(vel);
  }
  else if (padDown) //Para atrás
  {
    Serial.println("v");
    atras(vel);
  }
  else if (padRight) //Para derecha
  {
    Serial.println(">");
    derecha(vel);
  }
  else if (padLeft) //Para izquierda
  {
    Serial.println("<");
    izquierda(vel);
  }

}

//----------------------------------------------------

void motorOFF()
{
  digitalWrite(motFrenteA, LOW);
  digitalWrite(motAtrasA, LOW);
  digitalWrite(motFrenteB, LOW);
  digitalWrite(motAtrasB, LOW);
}

void atras(int velocidad)
{
  analogWrite(motFrenteA, velocidad);
  analogWrite(motAtrasA, LOW);
  analogWrite(motFrenteB, velocidad);
  analogWrite(motAtrasB, LOW);
}

void adelante(int velocidad)
{
  analogWrite(motFrenteA, LOW);
  analogWrite(motAtrasA, velocidad);
  analogWrite(motFrenteB, LOW);
  analogWrite(motAtrasB, velocidad);
}

void izquierda(int velocidad)
{
  analogWrite(motFrenteA, velocidad);
  analogWrite(motAtrasA, LOW);
  analogWrite(motFrenteB, LOW);
  analogWrite(motAtrasB, velocidad);
}

void derecha(int velocidad)
{
  analogWrite(motFrenteB, velocidad);
  analogWrite(motAtrasB, LOW);
  analogWrite(motFrenteA, LOW);
  analogWrite(motAtrasA, velocidad);
}
