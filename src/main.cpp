#include <Arduino.h>

const int trigPin1 = 5;
const int echoPin1 = 18;

const int trigPin2 = 0;
const int echoPin2 = 4;

const int channel = 0; // Canal PWM à utiliser
const int frequency = 50;// Fréquence PWM en Hz const
int resolution = 8; // Résolution PWM en bits (8 bits pour une sortie allant de 0 à 255)

const int pwmPin = 13; // Broche de sortie PWM

//define sound speed in cm/uSaaaa
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration1;
float distanceCm1;
float distanceInch1;

long duration2;
float distanceCm2;
float distanceInch2;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input

// Configuration de la bibliothèque ledc
ledcSetup(channel, frequency, resolution);
ledcAttachPin(pwmPin, channel);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echoPin1, HIGH);
  // Calculate the distance
  distanceCm1 = duration1 * SOUND_SPEED/2;
  // Convert to inches
  distanceInch1 = distanceCm1 * CM_TO_INCH;

  // Clears the trigPin
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
  // Calculate the distance
  distanceCm2 = duration2 * SOUND_SPEED/2;
  // Convert to inches
  distanceInch2 = distanceCm2 * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance1 (cm): ");
  Serial.println(distanceCm1);
  Serial.print("Distance1 (inch): ");
  Serial.println(distanceInch1);

  Serial.print("Distance2 (cm): ");
  Serial.println(distanceCm2);
  Serial.print("Distance2 (inch): ");
  Serial.println(distanceInch2);

  if(distanceCm1<30) {
    ledcWrite(channel, 13);
  }

  else {
    ledcWrite(channel, 23);
  }

  delay(1000);
}