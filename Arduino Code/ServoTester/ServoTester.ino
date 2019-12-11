#include <Servo.h>

const int SERVO_PIN = 3;
const int CRSERVOSWITCH_PIN = 2;
const int POTENTIOMETER_ANALOG_PIN = 0;
const bool POTENTIOMETER_DIRECTION_REVERSED = false;
const int POTENTIOMETER_CRSERVO_MARGIN_DEG = 20;
const int ERROR_ANGLE_MARGIN = 2;

Servo mServo = Servo();
int lastAng = 0;
bool lastCRServo = false;

void setup() {
  Serial.begin(9600);
  mServo.attach(SERVO_PIN);
  pinMode(CRSERVOSWITCH_PIN,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool isCRServo = digitalRead(CRSERVOSWITCH_PIN) == HIGH;
  int CRServoVal = analogRead(POTENTIOMETER_ANALOG_PIN);
  if(!POTENTIOMETER_DIRECTION_REVERSED)
    CRServoVal = 1023 - CRServoVal;

  int Ang = 0;
  int AngToWrite = 0;
  Ang = map(CRServoVal,0,1023,0,180);

  if(isCRServo && Ang >= 90 - POTENTIOMETER_CRSERVO_MARGIN_DEG && Ang <= 90 + POTENTIOMETER_CRSERVO_MARGIN_DEG){
    AngToWrite = 90;
  }else{
    AngToWrite = Ang;
  }

  mServo.write(AngToWrite);

  double FTCServoVal = ((double) Ang) / 180.0;
  double FTCCRServoSpeed = (((double) Ang) - 90.0) / 90.0;
  double FTCCRServoActualSpeed = (((double) AngToWrite) - 90.0) / 90.0;

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
