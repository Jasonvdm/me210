#define SHOOTER_VAR_INPUT A5
#define SHOOTER_MOTOR_OUTPUT 9
#define FEEDER_MOTOR_OUTPUT 10


void setup() 
{
    Serial.begin(9600);
  pinMode(SHOOTER_VAR_INPUT, INPUT);
  pinMode(SHOOTER_MOTOR_OUTPUT, OUTPUT);
  pinMode(FEEDER_MOTOR_OUTPUT, OUTPUT);
}

void loop() 
{
  int speedToShoot = analogRead(SHOOTER_VAR_INPUT)/4;
  Serial.println(speedToShoot);
  analogWrite(SHOOTER_MOTOR_OUTPUT, speedToShoot);
  analogWrite(FEEDER_MOTOR_OUTPUT, speedToShoot/2);
}
