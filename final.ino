#include <Servo.h>
Servo myservo;

int GRN_LED = 11; // blue signal wire, red live, black ground
int trigPin = 9;
int echoPin = 10;
int servoPin = 3;

int echoPinForBinStatus = 5;
int trigPinForBinStatus = 6;

int FORWARD = 110;
int NEUTRAL = 90;
int REVERSE = 70;

float duration, distanceForBinLead, distanceForBinStatus;
float distance;


void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin);
  myservo.write(NEUTRAL);

  pinMode(servoPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode (trigPinForBinStatus, OUTPUT);
  pinMode (echoPinForBinStatus, INPUT);

  pinMode(GRN_LED, OUTPUT);
}

// first USS check when to open bin
//second USS check when to notify that bin is full 
void loop() {

  distanceForBinLead = calculateDistance(trigPin, echoPin);
  distanceForBinStatus = calculateDistance(trigPinForBinStatus, echoPinForBinStatus);
  delay(200);

//when sensor for lead  is triggered
  if (distanceForBinLead <= 10) {
    
    myservo.write(REVERSE);  // Open bin
    delay(480);  // Wait for 4 seconds to make bin open
    myservo.write(NEUTRAL);
    //keeping bin open for 3 sec
    delay(5000);

    // Recheck after waiting to ensure the object is gone
    while (calculateDistance(trigPin, echoPin) <= 10) {
      // Keep the bin open while the object is still detected
      delay(3000);
    }

    // Once the object is out of range, start closing the bin
    closeBinSafely();
  }
  else {
    myservo.write(NEUTRAL);

  }

  //when sensor for bin is triggered
  if(distanceForBinStatus < 10){
    turnOnLED();
  }
  else{
    turnOffLED();
  }
}

void closeBinSafely() {
  myservo.write(FORWARD);
  delay(400);
  // After closing without detecting any object, set servo to neutral
  myservo.write(NEUTRAL);
}

  

void turnOnLED() {
  analogWrite(GRN_LED, 100);
}

void turnOffLED() {
  analogWrite(GRN_LED, LOW);
}

float calculateDistance(int trigPin, int echoPin) {
  // Making sure trigPin is in low state
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;
  Serial.print("Distance : ");
  Serial.println(distance);
  return distance;
}
