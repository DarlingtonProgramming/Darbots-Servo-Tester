#include <Servo.h>

const int SERVO_PIN = 3;
const int CRSERVOSWITCH_PIN = 2;
const int POTENTIOMETER_ANALOG_PIN = 0;
const bool POTENTIOMETER_DIRECTION_REVERSED = true;
const int POTENTIOMETER_CRSERVO_MARGIN = 50;

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
  if(POTENTIOMETER_DIRECTION_REVERSED)
    CRServoVal = 1023 - CRServoVal;

  int Ang = 0;
  if(isCRServo && (CRServoVal >= (1024/2) - POTENTIOMETER_CRSERVO_MARGIN) && (CRServoVal <= (1024/2) + POTENTIOMETER_CRSERVO_MARGIN)){
    Ang = 90;
  }else{
    Ang = map(CRServoVal,0,1023,0,180);
  }

  mServo.write(Ang);

  double FTCServoVal = ((double) Ang) / 180.0;
  double FTCCRServoSpeed = (((double) Ang) - 90.0) / 90.0;

  if(lastAng != Ang || lastCRServo != isCRServo){
    if(!isCRServo){
      Serial.print("Servo: ");
      Serial.print(Ang);
      Serial.print(" deg, ");
      Serial.println(FTCServoVal);
    }else{
      Serial.print("CRServo: ");
      Serial.println(FTCCRServoSpeed);
    }
  }
  
  lastAng = Ang;
  lastCRServo = isCRServo;
}
