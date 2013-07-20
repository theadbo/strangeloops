#define PIR 7
#define loop_delay 250

int pirState = LOW;
int val = 0;
boolean executed = false;

String pirStr, inputString = "";

void setup()
{
  // intentionally left blank
}

void loop() {
  // read from ping sensors
  if (!executed) mySetup();
  
  val = ping(PIR);
  delay(5);

  
  //build serial strings for PINGs
  pirStr = "AA0:";
  pirStr = pirStr + val;
  pirStr = pirStr + "BB";
  
  //print the string
  Serial.print(pirStr);
  delay(5);
  // clear the serial output, reset the string and flag
  Serial.flush();
  pirStr = "";

  delay(loop_delay);
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


void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");  
}

void mySetup(){
  pinMode(PIR, INPUT);
  Serial.begin(9600);
  Serial.flush();
  executed = true;
  delay(500);
}


