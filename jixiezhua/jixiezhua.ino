#include<Servo.h> 
Servo m;
int ledPin = 13;            
int inputPin = 2;  
void setup() {
  pinMode(ledPin, OUTPUT);   
  pinMode(inputPin, INPUT);  
  m.attach(5);
}
void loop() {
  m.write(90);
  delay(5000);
  for(int i=1;i<60;i++)
  {
   m.write(90+i); 
   digitalWrite(ledPin, LOW);  
   int val = digitalRead(inputPin); 
  if (val == HIGH) {    
    m.write(90+i-6);
    digitalWrite(ledPin, HIGH);
    break;  
  } 
  delay(50);
  }
 delay(5000000);
}
