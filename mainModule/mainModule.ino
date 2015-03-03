 #include <Timers.h>
#include <Servo.h> 

/*---------------- Module Defines ---------------------------*/
#define MOTOR_LEFT_OUTPUT_DIR_PIN 2
#define MOTOR_LEFT_OUTPUT_PWM_PIN 3
#define MOTOR_RIGHT_OUTPUT_DIR_PIN 4
#define MOTOR_RIGHT_OUTPUT_PWM_PIN 5

#define TAPE_SENSOR_FRONT_RIGHT A1
#define TAPE_SENSOR_FRONT_LEFT A0
#define TAPE_SENSOR_BACK_RIGHT A2
#define TAPE_SENSOR_BACK_LEFT A3

#define SHOOTER_VAR_INPUT A5
#define SHOOTER_MOTOR_OUTPUT 6
#define FEEDER_MOTOR_OUTPUT 10

#define MOTOR_DIR_FORWARDS 1
#define MOTOR_DIR_BACKWARDS 0

#define RIGHT_BUMPER 12
#define BOTTOM_BUMPER 13
#define LEFT_BUMPER 10

#define ONE_SEC            1000
#define TIME_INTERVAL      ONE_SEC

#define LIGHT_THRESHOLD 40
#define LIGHT_THRESHOLD_BACK 5
#define LIGHT_THRESHOLD_FRONT 4

#define TEST_TAPE 0
#define FIND_LINE 1
#define BACKUP_TO_BUMPER 2
#define SHOOTING_ENABLED 3


struct motor
{
   double mtrSpeed;
   boolean isMotorOn;
   int motorDir;
   int pwmPin;
   int dirPin;
};
typedef struct motor Motor;

Motor leftMotor= {0, false, 1, MOTOR_LEFT_OUTPUT_PWM_PIN, MOTOR_LEFT_OUTPUT_DIR_PIN};
Motor rightMotor = {0, false, 1, MOTOR_RIGHT_OUTPUT_PWM_PIN, MOTOR_RIGHT_OUTPUT_DIR_PIN};

Servo myservo;
int pos;

Servo reloader;
int reloaderPos = 0;

void initMotors();
void setMotorSpeed(Motor *motor, float mtrSpeed);
boolean isMotorOn(Motor motor);
void turnMotorOn(Motor *motor);
void turnMotorOff(Motor *motor);
void setMotorDir(Motor *motor, int dir);
void moveForwards();
void moveBackwards();
void turnAndFindLine();
void rightMotorOn(int value, int dir);
void rightMotorOff();
void leftMotorOn(int value, int dir);
void leftMotorOff();
boolean sensorOnTape(int value);
void turnOnShooter();
void moveBackwardsToFindWall();
void turnOnWhacker();
boolean sensorOnTapeFront(int value);
boolean sensorOnTapeBack(int value);
void testTapeSensors();
void getOnLine();
void findLineAndBumper();

int state = 0;
boolean shouldMove;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_LEFT_OUTPUT_DIR_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_OUTPUT_PWM_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_OUTPUT_DIR_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_OUTPUT_PWM_PIN, OUTPUT);
  
  pinMode(TAPE_SENSOR_FRONT_LEFT, INPUT);
  pinMode(TAPE_SENSOR_FRONT_RIGHT, INPUT);
  pinMode(TAPE_SENSOR_BACK_LEFT, INPUT);
  pinMode(TAPE_SENSOR_BACK_RIGHT, INPUT);
  
  pinMode(SHOOTER_VAR_INPUT, INPUT);
  pinMode(SHOOTER_MOTOR_OUTPUT, OUTPUT);
  pinMode(FEEDER_MOTOR_OUTPUT, OUTPUT);
  
  pinMode(LEFT_BUMPER, INPUT);
  pinMode(RIGHT_BUMPER, INPUT);
  
  reloader.attach(8); 
    
  initMotors();
  shouldMove = true;
}

void loop() 
{  
//    int x2 = analogRead(TAPE_SENSOR_FRONT_RIGHT);
//    int x1 = analogRead(TAPE_SENSOR_FRONT_LEFT);
//    int backLeftVal = analogRead(TAPE_SENSOR_BACK_LEFT);
//    int backRightVal = analogRead(TAPE_SENSOR_BACK_RIGHT);
//
//    Serial.print("Back left:");    
//    Serial.print(backLeftVal);
//    Serial.print("     ");
//    Serial.print("Back right:"); 
//    Serial.print(backRightVal);
//    Serial.print("     ");
//    Serial.print(x2);
//    Serial.print("     ");
//    Serial.print(x1);
//    Serial.println(" ");
//    delay(500);
//    shouldMove = false;
//      

state

switch (state)
{
  case TEST_TAPE:
    testTapeSensors();
    break;
 case FIND_LINE:
     findLineAndBumper();
     break;
 case SHOOTING_ENABLED:
     shootingEnabled();
     break;
}

//shouldMove = false;
//    if (shouldMove)
//    {
////      moveBackwardsToFindWall();
//      moveForwards();
//     int x2 = analogRead(TAPE_SENSOR_FRONT_RIGHT);
//     int x1 = analogRead(TAPE_SENSOR_FRONT_LEFT);
//       
//     if (x1 < LIGHT_THRESHOLD_FRONT || x2 < LIGHT_THRESHOLD_FRONT)
//     {       
//              motorStop();
//       shouldMove = false;
//       delay(1000);
//       turnAndFindLine();
//       delay(1000);
//       moveBackwardsToFindWall();
//       shouldMove = false;
//     } 
////     while (true)
////     {
////       turnOnWhacker();
////     }
//   } else {
////       turnOnShooter();
//       reloadAndShoot();
//    }
}

void shootingEnabled()
{
  turnOnShooter();
  reloadAndShoot();
}

void findLineAndBumper()
{
   moveForwards();
     int x2 = analogRead(TAPE_SENSOR_FRONT_RIGHT);
     int x1 = analogRead(TAPE_SENSOR_FRONT_LEFT);
       
     if (x1 < LIGHT_THRESHOLD_FRONT || x2 < LIGHT_THRESHOLD_FRONT)
     {       
       motorStop();
       delay(1000);
       turnAndFindLine();
       delay(1000);
       moveBackwardsToFindWall();
       state = SHOOTING_ENABLED;
     } 
}

void getOnLine()
{
  motorStop();
  while (true)
  {
    int frontRightVal = analogRead(TAPE_SENSOR_FRONT_RIGHT);
    int frontLeftVal = analogRead(TAPE_SENSOR_FRONT_LEFT);
    int backLeftVal = analogRead(TAPE_SENSOR_BACK_RIGHT);
    int backRightVal = analogRead(TAPE_SENSOR_BACK_LEFT);
    
    if ((sensorOnTapeFront(frontRightVal) || sensorOnTapeFront(frontLeftVal)) && sensorOnTapeBack(backLeftVal) && sensorOnTapeBack(backRightVal))
    {
      rightMotorOff();
      leftMotorOff();
      break;
    }
    
    if ((sensorOnTapeFront(frontRightVal) || sensorOnTapeFront(frontLeftVal)) && sensorOnTapeBack(backLeftVal) && !sensorOnTapeBack(backRightVal))
    {
        leftMotorOn(100, MOTOR_DIR_FORWARDS);
        rightMotorOn(155, MOTOR_DIR_BACKWARDS);
    } else if ((sensorOnTapeFront(frontRightVal) || sensorOnTapeFront(frontLeftVal)) && !sensorOnTapeBack(backLeftVal) && sensorOnTapeBack(backRightVal))
    {
        rightMotorOn(120, MOTOR_DIR_FORWARDS);
        leftMotorOn(120, MOTOR_DIR_BACKWARDS);
    } 
    
  }
}

void testTapeSensors()
{
    int x2 = analogRead(TAPE_SENSOR_FRONT_RIGHT);
    int x1 = analogRead(TAPE_SENSOR_FRONT_LEFT);
    int backLeftVal = analogRead(TAPE_SENSOR_BACK_LEFT);
    int backRightVal = analogRead(TAPE_SENSOR_BACK_RIGHT);

    Serial.print("Back left:");    
    Serial.print(backLeftVal);
    Serial.print("     ");
    Serial.print("Back right:"); 
    Serial.print(backRightVal);
    Serial.print("     ");
    Serial.print(x2);
    Serial.print("     ");
    Serial.print(x1);
    Serial.println(" ");
    delay(500);  
}

void moveBackwardsToFindWall()
{
  rightMotorOn(160, MOTOR_DIR_BACKWARDS);
  leftMotorOn(130, MOTOR_DIR_BACKWARDS);
  while (true){
    int frontRightVal = analogRead(TAPE_SENSOR_FRONT_RIGHT);
    int frontLeftVal = analogRead(TAPE_SENSOR_FRONT_LEFT);
    int backLeftVal = analogRead(TAPE_SENSOR_BACK_LEFT);
    int backRightVal = analogRead(TAPE_SENSOR_BACK_RIGHT);
    
    if ((sensorOnTapeFront(frontRightVal) || sensorOnTapeFront(frontLeftVal)) && sensorOnTapeBack(backLeftVal) && !sensorOnTapeBack(backRightVal))
    {
        rightMotorOn(165, MOTOR_DIR_BACKWARDS);
        
        int lastVal = backRightVal;
        while (!sensorOnTapeBack(lastVal))
        {
          lastVal = analogRead(TAPE_SENSOR_BACK_RIGHT);
        }
        rightMotorOn(160, MOTOR_DIR_BACKWARDS);        
    }
    
    if ((sensorOnTapeFront(frontRightVal) || sensorOnTapeFront(frontLeftVal)) && !sensorOnTapeBack(backLeftVal) && sensorOnTapeBack(backRightVal))
    {
        leftMotorOn(135, MOTOR_DIR_BACKWARDS);
        int lastVal = backLeftVal;
        while (!sensorOnTapeBack(lastVal))
        {
          lastVal = analogRead(TAPE_SENSOR_BACK_LEFT);
        }
        leftMotorOn(130, MOTOR_DIR_BACKWARDS);        
    }

    if (digitalRead(LEFT_BUMPER) && !digitalRead(RIGHT_BUMPER))
    {
      rightMotorOn(200, MOTOR_DIR_BACKWARDS);
      leftMotorOn(135, MOTOR_DIR_BACKWARDS);
      leftMotorOff();
    }
    if (!digitalRead(LEFT_BUMPER) && digitalRead(RIGHT_BUMPER))
    {
      leftMotorOn(200, MOTOR_DIR_BACKWARDS);
      rightMotorOn(135, MOTOR_DIR_BACKWARDS);      
      rightMotorOff();
    }
    
    if (digitalRead(LEFT_BUMPER) && digitalRead(RIGHT_BUMPER))
    {
      motorStop();
      break;  
    }
  }
}

void turnAndFindLine()
{
  boolean isMovingBackwards = false;
  rightMotorOn(150, MOTOR_DIR_FORWARDS);
  leftMotorOn(100, MOTOR_DIR_FORWARDS);
  while (true)
  {   
     int frontRightVal = analogRead(TAPE_SENSOR_FRONT_RIGHT);
     int frontLeftVal = analogRead(TAPE_SENSOR_FRONT_LEFT);
     int backLeftVal = analogRead(TAPE_SENSOR_BACK_LEFT);
     int backRightVal = analogRead(TAPE_SENSOR_BACK_RIGHT);
     
     if (digitalRead(LEFT_BUMPER) && digitalRead(RIGHT_BUMPER))
    {
      Serial.println("BOTH DEPRESSED");
      rightMotorOff();
      leftMotorOff();
      break;  
    }
    
    if (digitalRead(LEFT_BUMPER) && !digitalRead(RIGHT_BUMPER))
    {
      rightMotorOn(150, MOTOR_DIR_BACKWARDS);
      leftMotorOff();
    }
    if (!digitalRead(LEFT_BUMPER) && digitalRead(RIGHT_BUMPER))
    {
      leftMotorOn(150, MOTOR_DIR_BACKWARDS);
      rightMotorOff();
    }
    if ((sensorOnTapeFront(frontRightVal) || sensorOnTapeFront(frontLeftVal)) && sensorOnTapeBack(backLeftVal) && sensorOnTapeBack(backRightVal))
    {
      Serial.println("Terminating");
      rightMotorOff();
      leftMotorOff();
      break;
    }
    
    if ((!sensorOnTapeBack(backLeftVal) || !sensorOnTapeBack(backRightVal)) && isMovingBackwards)
    {
      isMovingBackwards = false;
      rightMotorOn(140, MOTOR_DIR_FORWARDS);
    }

     
     if (sensorOnTapeBack(backLeftVal) || sensorOnTapeBack(backRightVal) && !isMovingBackwards)
     {
       motorStop();
       rightMotorOn(150, MOTOR_DIR_BACKWARDS);
       leftMotorOn(130, MOTOR_DIR_BACKWARDS);
       isMovingBackwards = true;
//       while (sensorOnTape(backLeftVal) || sensorOnTape(backRightVal))
//       {
//         rightMotorOn(120, MOTOR_DIR_BACKWARDS);
//         leftMotorOn(120, MOTOR_DIR_BACKWARDS);
//       }
//       rightMotorOn(130, MOTOR_DIR_FORWARDS);
//       leftMotorOn(100, MOTOR_DIR_BACKWARDS);
//     } else {
//       rightMotorOn(130, MOTOR_DIR_FORWARDS);
//       leftMotorOn(100, MOTOR_DIR_BACKWARDS);
     }
//      rightMotorOn(130, MOTOR_DIR_FORWARDS);
//  leftMotorOn(100, MOTOR_DIR_BACKWARDS);

  }
}

boolean sensorOnTape(int value)
{
  if (value < LIGHT_THRESHOLD)
  {
    return true;
  }
  return false;
}

boolean sensorOnTapeBack(int value)
{
  if (value < LIGHT_THRESHOLD_BACK)
  {
    return true;
  }
  return false;
}

boolean sensorOnTapeFront(int value)
{
  if (value < LIGHT_THRESHOLD_FRONT)
  {
    return true;
  }
  return false;
}



void rightMotorOn(int value, int dir)
{
   setMotorDir(&rightMotor, dir);
   setMotorSpeed(&rightMotor, value);
}

void leftMotorOn(int value, int dir)
{
  setMotorDir(&leftMotor, dir);
  setMotorSpeed(&leftMotor, value);
}

void leftMotorOff()
{
  setMotorSpeed(&leftMotor, 0);
}

void rightMotorOff()
{
  setMotorSpeed(&rightMotor, 0);
}

void moveForwards()
{
  setMotorDir(&leftMotor, 1);
  setMotorDir(&rightMotor, 1);
  setMotorSpeed(&leftMotor, 150);
  setMotorSpeed(&rightMotor, 180);
}

void motorStop()
{
  setMotorSpeed(&leftMotor, 0);
  setMotorSpeed(&rightMotor, 0);
}

void moveBackwards()
{
  setMotorDir(&leftMotor, MOTOR_DIR_BACKWARDS);
  setMotorDir(&rightMotor, MOTOR_DIR_BACKWARDS);
  setMotorSpeed(&leftMotor, 140);
  setMotorSpeed(&rightMotor, 140);
}

void initMotors()
{
  turnMotorOn(&leftMotor);
  turnMotorOn(&rightMotor);  
}

void setMotorSpeed(Motor *motor, float mtrSpeed)
{

  if (motor->isMotorOn)
  {
    motor->mtrSpeed = mtrSpeed;
    analogWrite(motor->pwmPin, mtrSpeed);
  }
  
}

void turnMotorOn(Motor *motor)
{
  motor->isMotorOn = true;
}

void turnMotorOff(Motor *motor)
{
  motor->isMotorOn = false;
}

boolean isMotorOn(Motor motor)
{
  return motor.isMotorOn;
}

void setMotorDir(Motor *motor, int dir)
{
  motor->motorDir = dir;
  digitalWrite(motor->dirPin, dir);
}


void turnOnShooter()
{
  Serial.print("Shooter on");  
  int speedToShoot = analogRead(SHOOTER_VAR_INPUT)/4;
  Serial.println(speedToShoot);
  analogWrite(SHOOTER_MOTOR_OUTPUT, speedToShoot);
  analogWrite(FEEDER_MOTOR_OUTPUT, 255);
}

void reloadAndShoot()
{
  for(reloaderPos = 15; reloaderPos <= 90; reloaderPos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    reloader.write(reloaderPos);              // tell servo to go to position in variable 'pos' 
    delay(6);                       // waits 15ms for the servo to reach the position 
  } 

  for(reloaderPos = 90; reloaderPos>=15; reloaderPos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    reloader.write(reloaderPos);              // tell servo to go to position in variable 'pos' 
    delay(6);                       // waits 15ms for the servo to reach the position 
  } 
    delay(1500);
}

void turnOnWhacker()
{
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(2);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(2);                       // waits 15ms for the servo to reach the position 
  } 
}


unsigned char TestForKey(void) {
  unsigned char KeyEventOccurred;
  KeyEventOccurred = Serial.available();
  Serial.read();
  return KeyEventOccurred;
}

void RespToKey(void) {
  
}


