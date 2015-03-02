 #include <Timers.h>

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
#define SHOOTER_MOTOR_OUTPUT 9
#define FEEDER_MOTOR_OUTPUT 10

#define MOTOR_DIR_FORWARDS 1
#define MOTOR_DIR_BACKWARDS 0

#define ONE_SEC            1000
#define TIME_INTERVAL      ONE_SEC

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
  
  initMotors();
  shouldMove = false;
}

void loop() 
{  
//    int x2 = analogRead(TAPE_SENSOR_FRONT_RIGHT);
//    int x1 = analogRead(TAPE_SENSOR_FRONT_LEFT);
//    int backLeftVal = analogRead(TAPE_SENSOR_BACK_LEFT);
//    int backRightVal = analogRead(TAPE_SENSOR_BACK_RIGHT);
//    
//    Serial.print(backLeftVal);
//    Serial.print("     ");
//    Serial.print(backRightVal);
//    Serial.print("     ");
//    Serial.print(x2);
//    Serial.print("     ");
//    Serial.print(x1);
//    Serial.println(" ");
//    delay(500);
//    shouldMove = false;

int speedToShoot = analogRead(SHOOTER_VAR_INPUT)/4;
  Serial.println(speedToShoot);
  analogWrite(SHOOTER_MOTOR_OUTPUT, speedToShoot);
  analogWrite(FEEDER_MOTOR_OUTPUT, speedToShoot/2);
  
    if (shouldMove)
    {
      moveForwards();
     int x2 = analogRead(TAPE_SENSOR_FRONT_RIGHT);
     int x1 = analogRead(TAPE_SENSOR_FRONT_LEFT);
     
     if (x1 < 700 && x2 < 700)
     {
       shouldMove = false;
       delay(100);
       motorStop();
       delay(500);
       turnAndFindLine();
     }   
   }
//  if (shouldMove)
//  {
//    if (TMRArd_IsTimerActive(0) == TMRArd_NOT_ACTIVE)
//    {
//        TMRArd_InitTimer(0, 0); 
//        TMRArd_SetTimer(0, TIME_INTERVAL*5);
//        TMRArd_StartTimer(0);
//    }   
//    if (TMRArd_IsTimerExpired(0) == TMRArd_EXPIRED) {
//        shouldMove = false;
//        TMRArd_ClearTimerExpired(0);
//        
//    } else {
//        moveForwards();
//    }
//  } else {
//    
//     motorStop();
//     if (TMRArd_IsTimerActive(1) == TMRArd_NOT_ACTIVE)
//    {
//        TMRArd_InitTimer(1, 0); 
//        TMRArd_SetTimer(1, TIME_INTERVAL);
//        TMRArd_StartTimer(1);
//    }   
//    if (TMRArd_IsTimerExpired(1) == TMRArd_EXPIRED) {
//        shouldMove = true;
//        TMRArd_ClearTimerExpired(1);
//        
//    }
//  }
  
//    } else if(TMRArd_IsTimerExpired(0) == TMRArd_EXPIRED) {
//      Serial.println("TIME IS EXPIRED");
//      state = 1;
//    }
////    
//  } else {
//    motorStop();
//     if (TMRArd_IsTimerExpired(1) == TMRArd_EXPIRED && TMRArd_IsTimerActive(1) == TMRArd_NOT_ACTIVE) {
//        TMRArd_InitTimer(1, TIME_INTERVAL);
//    } else if(TMRArd_IsTimerExpired(1) == TMRArd_EXPIRED) {
//      state = 0;
//    }
//  }
  
  
 
    
}

void turnAndFindLine()
{
  boolean isMovingBackwards = false;
  rightMotorOn(130, MOTOR_DIR_FORWARDS);
//  leftMotorOn(100, MOTOR_DIR_BACKWARDS);
  while (true)
  {   
     int frontRightVal = analogRead(TAPE_SENSOR_FRONT_RIGHT);
     int frontLeftVal = analogRead(TAPE_SENSOR_FRONT_LEFT);
     int backLeftVal = analogRead(TAPE_SENSOR_BACK_RIGHT);
     int backRightVal = analogRead(TAPE_SENSOR_BACK_LEFT);
     
    if ((sensorOnTape(frontRightVal) && sensorOnTape(frontLeftVal)) && sensorOnTape(backLeftVal) && sensorOnTape(backRightVal))
    {
      Serial.println("Terminating");
      rightMotorOff();
      leftMotorOff();
      break;
    }
    
    if (!sensorOnTape(backLeftVal) && !sensorOnTape(backRightVal) && isMovingBackwards)
    {
      isMovingBackwards = false;
      rightMotorOn(100, MOTOR_DIR_FORWARDS);
    }

     
     if (sensorOnTape(backLeftVal) || sensorOnTape(backRightVal) && !isMovingBackwards)
     {
       motorStop();
       rightMotorOn(150, MOTOR_DIR_BACKWARDS);
       leftMotorOn(120, MOTOR_DIR_BACKWARDS);
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
  if (value < 700)
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
  setMotorSpeed(&leftMotor, 255);
  setMotorSpeed(&rightMotor, 255);
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
  int speedToShoot = analogRead(SHOOTER_VAR_INPUT)/4;
  Serial.println(speedToShoot);
  analogWrite(SHOOTER_MOTOR_OUTPUT, speedToShoot);
  analogWrite(FEEDER_MOTOR_OUTPUT, speedToShoot/2);
}






unsigned char TestForKey(void) {
  unsigned char KeyEventOccurred;
  KeyEventOccurred = Serial.available();
  Serial.read();
  return KeyEventOccurred;
}

void RespToKey(void) {
  
}


