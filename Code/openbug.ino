#include <Servo.h>
Servo myservo;
void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  myservo.attach(A1);
  pinMode(A0, INPUT);
}

// the loop routine runs over and over again forever:

void loop() {

  int sensorValue = analogRead(A0);
  if(sensorValue > 900){
    myservo.write(0);
    delay(1000);
    }
  else{
    myservo.write(180);
    delay(5000);
    }
  Serial.println(sensorValue);
}
