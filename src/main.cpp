#include <Arduino.h>
#include <Wire.h>

const int trigPin1 = 5;
const int echoPin1 = 18;

const int trigPin2 = 4;
const int echoPin2 = 0;

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
int result;


bool vehiculeEntrant = false;
bool barriereOccupee = false;

static int angle = 0;

void fermerBarriere(void)
{
  while (angle != 13)
  {
    if (angle<13)
    {
      angle++;
      ledcWrite(channel,angle);
    }
    else if (angle>13)
    {
      angle--;
      ledcWrite(channel,angle);
    }
    delay(500);
  }
}

void ouvrirBarriere(void)
{
  while (angle != 20)
  {
    if (angle<20)
    {
      angle++;
      ledcWrite(channel,angle);
    }
    else if (angle>20)
    {
      angle--;
      ledcWrite(channel,angle);
    }
    delay(500);
  }
}



int lireDistance(void)
{
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
  return distanceCm1;
}

void receiveEvent(int howMany){
  byte myByte = Wire.read(); // Read the incoming byte
  Serial.print("receivedByte: "); // Print the byte to the serial monitor
  Serial.println(myByte); // Print the byte to the serial monitor
  printf("receive");
  if (myByte == 10)
  {
    Serial.print("if 10 ");
    distanceCm1 = lireDistance();
    if(distanceCm1>20) result = 10;
    else result = 5;
  }

}

void requestEvent(){
  /*Wire.write((int) distanceCm1);
  Wire.write((int) distanceCm1);*/
  
  Wire.write(result);
}

void setup() {
  Wire.begin(0x54); // Rejoindre le bus à l'adresse #13
	Wire.onReceive(receiveEvent); // Preparer une fonction spécifique a la reception de donnee
  Wire.onRequest(requestEvent);
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input

  // Configuration de la bibliothèque ledc
  ledcSetup(channel, frequency, resolution);
  ledcAttachPin(pwmPin, channel);
  angle = 19;
  ouvrirBarriere();
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

  if(distanceCm1<20) {
    fermerBarriere();
    vehiculeEntrant = true;
  }
  else if(vehiculeEntrant) {
    if(distanceCm2<20) {
      fermerBarriere();
      barriereOccupee = true;
    }
    else if((barriereOccupee) and (vehiculeEntrant)) {
      ouvrirBarriere();
      vehiculeEntrant = false;
      barriereOccupee = false;
    }
    else {
      fermerBarriere();
    }
  }
  else {
    ouvrirBarriere();
  }
    // Prints the distance in the Serial Monitor
  Serial.print("Distance1 (cm): ");
  Serial.println(distanceCm1);
  Serial.print("Distance1 (inch): ");
  Serial.println(distanceInch1);

  Serial.print("Distance2 (cm): ");
  Serial.println(distanceCm2);
  Serial.print("Distance2 (inch): ");
  Serial.println(distanceInch2);

  Serial.print("vehiculeEntrant: ");
  Serial.println(vehiculeEntrant);
  Serial.print("barriereOccupee: ");
  Serial.println(barriereOccupee);

  Serial.print("position servo: ");
  Serial.println(ledcRead(channel));

  Serial.print("angle desire: ");
  Serial.println(ledcRead(angle));

  delay(1000);


  // Prints the distance in the Serial Monitor
 /* Serial.print("Distance1 (cm): ");
  Serial.println(distanceCm1);
  Serial.print("Distance1 (inch): ");
  Serial.println(distanceInch1);

  Serial.print("Distance2 (cm): ");
  Serial.println(distanceCm2);
  Serial.print("Distance2 (inch): ");
  Serial.println(distanceInch2);

  Serial.print("vehiculeEntrant: ");
  Serial.println(vehiculeEntrant);
  Serial.print("barriereOccupee: ");
  Serial.println(barriereOccupee);

  Serial.print("position servo: ");
  Serial.println(ledcRead(channel));

  Serial.print("angle desire: ");
  Serial.println(ledcRead(angle));*/

  delay(100);
}