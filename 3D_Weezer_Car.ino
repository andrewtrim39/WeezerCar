// Infrared Configurator
#include <IRremote.h>
int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;
// Motion Pinout Configurator
int frontAxelPlus = 3; int frontAxelMinus = 5;
int rearAxelPlus = 6;  int rearAxelMinus = 9;
int steerPlus = 10;    int steerMinus = 11;
// Lighting Pinout Configurator
int headlights = 2;   // White! Use AnalogRead to figure out voltage drop
int taillights = 8;   // Red! Use 220Ω resistor
int brakelights = 12; // Red! Use 220Ω resistor

int pinionPosition = 0;
bool h = true; bool t = true;

void setup() {Serial.begin(9600);
  pinMode(headlights, OUTPUT); 
  pinMode(taillights, OUTPUT);
  pinMode(brakelights, OUTPUT);
  irrecv.enableIRIn();}

/* 1   2   3     -   ^   -     0xFF30CF  0xFF18E7  0xFF7A85
 * 4   5   6  =  <   B   >  =  0xFF10EF  0xFF38C7  0xFF5AA5
 * 7   8   9     -   v   -     0xFF42BD  0xFF4AB5  0xFF52AD */
 
void loop(){if(irrecv.decode(&results)){
    Serial.println(results.value);
    switch(results.value){ 
      // Directional Commands
      case 0xFF18E7: goForwards();  break; // 2
      case 0xFF38C7: brake();       break; // 5
      case 0xFF4AB5: goBackwards(); break; // 8
      case 0xFF10EF: turnLeft();    break; // 4
      case 0xFF5AA5: turnRight();   break; // 6
      // Lighting Commands
      case 0xFF22DD: digitalWrite(headlights, h); h = !h; break;  // <<
      case 0xFF02FD: digitalWrite(taillights, t); t = !t; break;} // >>
    irrecv.resume();}}

void goForwards() {
  digitalWrite(brakelights, LOW);
  pinMode(frontAxelPlus, OUTPUT);
  pinMode(frontAxelMinus, INPUT);
  pinMode(rearAxelPlus, OUTPUT);
  pinMode(rearAxelMinus, INPUT);
  digitalWrite(frontAxelPlus, HIGH);
  digitalWrite(rearAxelPlus, HIGH);
}

void goBackwards() {
  digitalWrite(brakelights, HIGH);
  pinMode(frontAxelPlus, INPUT);
  pinMode(frontAxelMinus, OUTPUT);
  pinMode(rearAxelPlus, INPUT);
  pinMode(rearAxelMinus, OUTPUT);
  digitalWrite(frontAxelMinus, HIGH);
  digitalWrite(rearAxelMinus, HIGH);
}

void turnRight() {
  digitalWrite(brakelights, LOW);
  pinMode(steerPlus, OUTPUT);
  pinMode(steerMinus, INPUT);
  digitalWrite(steerPlus, HIGH);
  delay(500);
  digitalWrite(steerPlus, LOW);
  pinionPosition = pinionPosition + 1;
}

void turnLeft() {
  digitalWrite(brakelights, LOW);
  pinMode(steerPlus, INPUT);
  pinMode(steerMinus, OUTPUT);
  digitalWrite(steerMinus, HIGH);
  delay(500);
  digitalWrite(steerPlus, LOW);
  pinionPosition = pinionPosition - 1;
}

void brake() {
  digitalWrite(brakelights, HIGH);
  pinMode(frontAxelPlus, OUTPUT);
  pinMode(frontAxelMinus, INPUT);
  pinMode(rearAxelPlus, OUTPUT);
  pinMode(rearAxelMinus, INPUT);
  digitalWrite(frontAxelPlus, LOW);
  digitalWrite(rearAxelPlus, LOW);
  if (pinionPosition == 1) {
    turnLeft();
  }
  else if (pinionPosition == -1) {
    turnRight();
  }
}
