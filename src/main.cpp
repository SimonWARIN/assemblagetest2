#include <Arduino.h>

const int trigPin = 18;
const int echoPin = 5;

const int channel = 0; // Canal PWM à utiliser
const int frequency = 50;// Fréquence PWM en Hz const
int resolution = 8; // Résolution PWM en bits (8 bits pour une sortie allant de 0 à 255)

const int pwmPin = 13; // Broche de sortie PWM

//define sound speed in cm/uSaaaa
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

// Configuration de la bibliothèque ledc
ledcSetup(channel, frequency, resolution);
ledcAttachPin(pwmPin, channel);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculate the distance
distanceCm = duration * SOUND_SPEED/2;

// Convert to inches
distanceInch = distanceCm * CM_TO_INCH;

// Prints the distance in the Serial Monitor
Serial.print("Distance (cm): ");
Serial.println(distanceCm);
Serial.print("Distance (inch): ");
Serial.println(distanceInch);

if(distanceCm<30) {
  ledcWrite(channel, 13);
}

else {
  ledcWrite(channel, 23);
}

delay(1000);
}