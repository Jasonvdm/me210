#include <Servo.h> 

Servo myservo;
int pos = 0;

void setup() 
{ 
    Serial.begin(9600);
  myservo.attach(9); 
} 
 
void loop() 
{ 
  for(pos = 15; pos <= 90; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(6);                       // waits 15ms for the servo to reach the position 
  } 

  for(pos = 90; pos>=15; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(6);                       // waits 15ms for the servo to reach the position 
  } 
    delay(1500);
}
