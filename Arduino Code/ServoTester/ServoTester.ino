#include <Servo.h>

const int SERVO_PIN = 3;
const int CRSERVOSWITCH_PIN = 2;
const int POTENTIOMETER_ANALOG_PIN = 0;
const bool POTENTIOMETER_DIRECTION_REVERSED = false;
const int POTENTIOMETER_CRSERVO_MARGIN_DEG = 20;
const int ERROR_ANGLE_MARGIN = 2;
const int PWM_LOWER_RANGE_uS = 1000;
const int PWM_UPPER_RANGE_uS = 2000;
const double SERVO_MAX_ANG = 180.0;

const int PWM_RANGE_DELTA = PWM_UPPER_RANGE_uS - PWM_LOWER_RANGE_uS;

Servo mServo = Servo();
int lastAng = 0;
bool lastCRServo = false;

void setup() {
  Serial.begin(9600);
  mServo.attach(SERVO_PIN);
  pinMode(CRSERVOSWITCH_PIN,INPUT);
}

int roundNum(double number){
  return (int) (number + 0.5);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool isCRServo = digitalRead(CRSERVOSWITCH_PIN) == HIGH;
  int CRServoVal = analogRead(POTENTIOMETER_ANALOG_PIN);
  if(!POTENTIOMETER_DIRECTION_REVERSED)
    CRServoVal = 1023 - CRServoVal;

  double Ang = 0;
  double AngToWrite = 0;
  Ang = ((double) CRServoVal) / 1023.0 * SERVO_MAX_ANG; //map(CRServoVal,0,1023,0,180);

  if(isCRServo && Ang >= 90 - POTENTIOMETER_CRSERVO_MARGIN_DEG && Ang <= 90 + POTENTIOMETER_CRSERVO_MARGIN_DEG){
    AngToWrite = SERVO_MAX_ANG / 2.0;
  }else{
    AngToWrite = Ang;
  }

  int pulseToWrite = roundNum(AngToWrite / SERVO_MAX_ANG * PWM_RANGE_DELTA) + PWM_LOWER_RANGE_uS;

  mServo.writeMicroseconds(pulseToWrite);

  double FTCServoVal = ((double) Ang) / SERVO_MAX_ANG;
  double FTCCRServoSpeed = (((double) Ang) / SERVO_MAX_ANG) - 1.0;
  double FTCCRServoActualSpeed = (((double) AngToWrite) / SERVO_MAX_ANG) - 1.0;

  if(lastCRServo != isCRServo || ((lastAng - Ang) < -ERROR_ANGLE_MARGIN || (lastAng-Ang) > ERROR_ANGLE_MARGIN)){
    if(!isCRServo){
      Serial.println("Servo: ");
      Serial.print("  deg: ");
      Serial.println(Ang);
      Serial.print("  FTCVal: ");
      Serial.println(FTCServoVal);
    }else{
      Serial.println("CRServo: ");
      Serial.print("  Actual Speed Written: ");
      Serial.println(FTCCRServoActualSpeed);
      Serial.print("  Speed: ");
      Serial.println(FTCCRServoSpeed);
    }
    lastAng = Ang;
    lastCRServo = isCRServo;
  }
}
