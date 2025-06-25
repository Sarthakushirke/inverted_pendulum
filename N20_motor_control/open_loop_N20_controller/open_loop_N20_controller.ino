#define MotFwd  5  // Motor Forward pin
#define MotRev  6 // Motor Reverse pin
#define COUNTS_PER_REV 595.8  // from datasheet

unsigned long lastTime = 0;
float lastAngle = 0;
int encoderPin1 = 2; //Encoder Output 'A' must connected with intreput pin of arduino.
int encoderPin2 = 3; //Encoder Otput 'B' must connected with intreput pin of arduino.
volatile int lastEncoded = 0; // Here updated value of encoder store.
volatile long encoderValue = 0; // Raw encoder value



// Function to convert encoder value to radians
float getAngleRad() {
  return (2.0 * PI * encoderValue) / COUNTS_PER_REV;
}


float getAngularVelocity() {
  unsigned long currentTime = millis();
  float currentAngle = getAngleRad();
  float dt = (currentTime - lastTime) / 1000.0;  // in seconds

  float omega = (currentAngle - lastAngle) / dt;

  lastTime = currentTime;
  lastAngle = currentAngle;

  return omega;  // in rad/s
}

//Open loop control
float x = 6.0;  // target angle in radians (e.g., 90 degrees)
float theta_start = getAngleRad();
float theta_target = theta_start + x;


void setup() {

  pinMode(MotFwd, OUTPUT); 
  pinMode(MotRev, OUTPUT); 
  Serial.begin(9600); //initialize serial comunication

   pinMode(encoderPin1, INPUT_PULLUP); 
  pinMode(encoderPin2, INPUT_PULLUP);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  //attachInterrupt(0, updateEncoder, CHANGE); 
  //attachInterrupt(1, updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPin1), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPin2), updateEncoder, CHANGE);

}

void loop() {

 while (getAngleRad() < theta_target) {
  analogWrite(MotFwd, 0);  // or whatever PWM needed
  analogWrite(MotRev, 180);    // forward direction
   Serial.print("Theta target  ");
   Serial.println(theta_target);
   Serial.print("Radians  ");
   Serial.println(getAngleRad());
}

// Stop the motor
analogWrite(MotFwd, 0);
analogWrite(MotRev, 0);

//for (int i = 0; i <= 120; i++){
////digitalWrite(MotFwd, LOW); 
////digitalWrite(MotRev, HIGH);
//analogWrite(MotFwd, 0);  // 60% duty cycle
//analogWrite(MotRev, 100);    // Reverse off
//float omega = getAngularVelocity();  // rad/s
//float rpm = (omega * 60.0) / (2 * PI);  // convert to RPM
// Serial.print("Forward  ");
//// Serial.println(encoderValue);
// Serial.print("  |  Angle (rad): ");
// Serial.println(getAngleRad());
////Serial.print(" | Angular Velocity (rad/s): ");
////Serial.println(getAngularVelocity());
//
////    Serial.print(" | RPM: ");
////    Serial.println(rpm);
//}

//delay(10);

//for (int i = 0; i <= 120; i++){
////digitalWrite(MotFwd, HIGH); 
//// digitalWrite(MotRev, LOW);
//analogWrite(MotFwd, 100);  // 60% duty cycle
//analogWrite(MotRev, 0);    // Reverse off
////float omega = getAngularVelocity();  // rad/s
////float rpm = (omega * 60.0) / (2 * PI);  // convert to RPM
// Serial.print("Reverse  ");
//// Serial.println(encoderValue);
// Serial.print("  |  Angle (rad): ");
// Serial.println(getAngleRad());
////Serial.print(" | Angular Velocity (rad/s): ");
////Serial.println(getAngularVelocity());
////    Serial.print(" | RPM: ");
////    Serial.println(rpm);
//}
//
//delay(10);

} 

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue --;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue ++;

  lastEncoded = encoded; //store this value for next time

}
