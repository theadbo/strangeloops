#define PIR 7
#define PIR2 6
#define PIR3 5
#define LED 2
#define rPin 9
#define gPin 10
#define bPin 11
#define flexPin 1

int pirState = LOW;
int val, val2, val3, flx = 0;
int ack_track = 0;
int incoming = 0;
boolean ackFound, executed, errorLED_off = false;
String pirStr, pirStr2, pirStr3, flexStr, inputString = "";

void setup()
{
  // intentionally left blank
}

void loop() {
  // read from ping sensors
  if (!executed) mySetup();
  int current_time = millis();
  if ((current_time-ack_track)>10000){
    software_Reset();
  }
  val = ping(PIR);
  delay(5);
  digitalWrite(LED, LOW);
//  val2 = ping(PIR2);
//  delay(5);
//  val3 = ping(PIR3);
//  
//  // read from flex sensor
//  delay(5);
//  flx = map(flex(flexPin),0,1000,0,100);
//  flx = flex(flexPin);
  
  //build serial strings for PINGs
  pirStr = "0:";
  pirStr = pirStr + val;
  pirStr = pirStr + "\r\n";
//  pirStr2 = "1:";
//  pirStr2 = pirStr2 + val2;
//  pirStr2 = pirStr2 + "\r\n";
//  pirStr3 = "2:";
//  pirStr3 = pirStr3 + val3;
//  pirStr3 = pirStr3 + "\r\n";
//  
//  // grab sit sensor and build it's string
//  flexStr = "3:";
//  flexStr = flexStr + flx;
//  flexStr = flexStr + "\r\n";
  
    //the following code implements the ACK method of data request
    // print data to serial port, if requested
//  if (ackFound){
//    ack_track = millis();
//    Serial.print(pirStr);
//    delay(5);
//    Serial.print(pirStr2);
//    delay(5);
//    Serial.print(pirStr3);
//    delay(5);
//    Serial.print(flexStr);
    // clear the serial output, reset the string and flag
//    Serial.flush();
//    inputString = "";
//    ackFound = false;
//  }
//    // ************ //
//    //comment out this code when using ACK
    Serial.print(pirStr);
    delay(5);
//    Serial.print(pirStr2);
//    delay(5);
//    Serial.print(pirStr3);
//    delay(5);
//    Serial.print(flexStr);
//    // clear the serial output, reset the string and flag
    Serial.flush();
//    // **************//
  delay(250);
}

long ping(int pin) {
  if (!errorLED_off) digitalWrite(LED, HIGH);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  
  long duration = pulseIn(pin, HIGH, 20000);
  if (duration==0) errorLED_off = true;
  //Serial.println(duration);

  return (microsecondsToCentimeters(duration));
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

long flex(int pin)
{
  int flexReading = analogRead(flexPin);

  return (flexReading);
}
//void fadeOut() {
//  
//}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, check
    // to see if the string was an ACK
    if (inChar == '\n') {
      if (inputString.equals("ACK\n"))
      ackFound = true;
    } 
  }
}

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");  
}

void mySetup(){
  pinMode(PIR, INPUT);
  pinMode(PIR2, INPUT);
  pinMode(PIR3, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
  executed = true;
  delay(500);
}


