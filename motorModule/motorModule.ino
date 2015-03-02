

struct motor
{
   double mtrSpeed;
   boolean isMotorOn = false;
   int outputPin;
};
typedef struct motor Motor;


void initMotor(Motor motor, int outputPin)
{
  motor.outputPin = outputPin;
  pinMode(outputPin, OUTPUT);
}

void setMotorSpeed(Motor motor, double mtrSpeed)
{
  motor.mtrSpeed = mtrSpeed;
}

void turnMotorOn(Motor motor)
{
  motor.isMotorOn = true;
}

void turnMotorOff(Motor motor)
{
  motor.isMotorOn = false;
}

boolean isMotorOn(Motor motor)
{
  return motor.isMotorOn;
}


