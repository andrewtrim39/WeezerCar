// Setting up the Infrared Remote
#include <IRremote.h> 
const int RECV_PIN = 7; IRrecv irrecv(RECV_PIN); decode_results results;
// Setting up the motor outputs (digital)
const int frontWheelDrive =  3; const int rearWheelDrive = 5;
const int steeringPinionNegative = 6; const int steeringPinionPositive = 9;
// Setting up the joystick pinouts (analog)
const int LeftRightPlus = 1; const int UpDownPlus = 3;
int x = 0; int y = 0;
// Setting up digital pinouts for headlights and taillights
const int leftHeadlight = 4; /* white*/ const int rightHeadlight = 7;  /* white*/
const int leftTaillight = 8; /* red  */ const int rightTaillight = 10; /* red  */
const int brakeLights = 11;  /* red  */

void setup(){Serial.begin(9600);
  pinMode(frontWheelDrive, OUTPUT);pinMode(rearWheelDrive, OUTPUT);
  irrecv.enableIRIn();
  pinMode(leftTaillight, OUTPUT); pinMode(rightTaillight, OUTPUT);
  pinMode(leftHeadlight, OUTPUT); pinMode(rightHeadlight, OUTPUT);}

void loop() {
  int LeftRightJoystick = analogRead(LeftRightPlus); int UpDownJoystick = analogRead(UpDownPlus);
  // When it's going forward or turning right
  if (y >= 0 && y < 256 && x >= 0 && x < 256){allLightsOn();
    pinMode(steeringPinionPositive, OUTPUT);
    pinMode(steeringPinionNegative, INPUT);
    switch(results.value){
      case 0xFF18E7: y = y + 5; analogWrite(frontWheelDrive, y); analogWrite(rearWheelDrive, y); break;
      case 0xFF4AB5: y = y - 5; analogWrite(frontWheelDrive, y); analogWrite(rearWheelDrive, y); break;
      case 0xFF10EF: x = x - 5; analogWrite(steeringPinionPositive, x); break;
      case 0xFF5AA5: x = x + 5; analogWrite(steeringPinionPositive, x); break;}}
  // When it's turning left
  else if (x < 0 && x > -256){allLightsOn();
    pinMode(steeringPinionNegative, OUTPUT); pinMode(steeringPinionPositive, INPUT);
    switch(results.value){
      case 0xFF18E7: y = y + 5; analogWrite(frontWheelDrive, y); analogWrite(rearWheelDrive, y); break;
      case 0xFF4AB5: y = y - 5; analogWrite(frontWheelDrive, y); analogWrite(rearWheelDrive, y); break;
      case 0xFF10EF: x = x - 5; analogWrite(steeringPinionPositive, (0 - x)); break;
      case 0xFF5AA5: x = x + 5; analogWrite(steeringPinionPositive, (0 - x)); break;}}
  // Preventative function to stop y from becoming negative because it's not programmed to go backwards
  else if(y < 0){y = 0;
    analogWrite(frontWheelDrive, y); analogWrite(rearWheelDrive, y);}
  // Preventative function to stop y from exceeding its maximum value as an analog value
  else if(y > 255){y = 255;
    analogWrite(frontWheelDrive, y); analogWrite(rearWheelDrive, y);}
  // Preventative to stop x from exceeding it's maximum analog value when steering left
  else if (x < -256){x = -255;
    pinMode(steeringPinionNegative, OUTPUT); pinMode(steeringPinionPositive, INPUT);
    analogWrite(steeringPinionNegative, (0 - 255));}
  // prevents x from exceeding maximum analog value 
  else if (x > 256){x = 255;
    pinMode(steeringPinionNegative, INPUT); pinMode(steeringPinionPositive, OUTPUT);
    analogWrite(steeringPinionPositive, x);}
  else if (y == 0){allLightsOn(); digitalWrite(brakeLights, HIGH);}}
//------------------------------------------------------------------------------------------------------
void allLightsOn(){
  digitalWrite(rightHeadlight, HIGH); digitalWrite(leftHeadlight, HIGH);
  digitalWrite(rightTaillight, HIGH); digitalWrite(leftTaillight, HIGH);}
