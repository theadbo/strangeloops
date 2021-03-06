#define PIR 7

int pirState = LOW;
int val,smthd_val = 0;
int filter_val = 0.5;
boolean executed = false;

//uncomment when using ack
int ack_track = 0;
boolean ackFound = false;

String pirStr, inputString = "";

void setup()
{
  // intentionally left blank
}

void loop() {
  // read from ping sensors
  if (!executed) mySetup();
  
//  uncomment when using ack  
  int current_time = millis();
  if ((current_time-ack_track)>10000){
    software_Reset();
  }

  val = ping(PIR);
  smthd_val = smooth(val, filter_val, smthd_val);
  delay(5);

  
  //build serial strings for PINGs
  pirStr = "0:";
  pirStr = pirStr + smthd_val;
  pirStr = pirStr + "\r\n";

  
    //the following code implements the ACK method of data request
    // print data to serial port, if requested
  if (ackFound){
    ack_track = millis();
    Serial.print(pirStr);
    delay(5);
    // clear the serial output, reset the string and flag
    Serial.flush();
    inputString = "";
    // set the ackFound boolean back to false so we don't read when
    // we don't want to
    ackFound = false;
  }
//    // ************ //
//    //comment out this code when using ACK
//    Serial.print(pirStr);
//    delay(5);
//    Serial.print(pirStr2);
//    delay(5);
//    Serial.print(pirStr3);
//    delay(5);
//    Serial.print(flexStr);
//    // clear the serial output, reset the string and flag
//    Serial.flush();
//    // **************//
  delay(250);
}

long ping(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  
  long duration = pulseIn(pin, HIGH, 20000);
  
  //Serial.println(duration); //debugging

  return (microsecondsToCentimeters(duration));
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}


// uncomment when using ack
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
  Serial.begin(9600);
  executed = true;
  delay(500);
}

int smooth(int data, float filterVal, float smoothedVal){


  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}


