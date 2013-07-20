/* ColourTester

   Write arbitrary colour values to an RGB LCD.

   To use, upload sketch, open serial monitor, and
   set line endings to "Newline".
   Enter three numbers between 0-255, separated by spaces.
   Hit Send. The arduino will respond with the values, and
   light up the LED.
*/

/* DIODER colours:
   Pin 9: Green
   Pin 10: Blue
   Pin 11: Red
*/

int rLedPin=9;
int gLedPin=10;
int bLedPin=11;

char serialByte;
char serialBuffer[32];
int serialIndex = 0;

int rValue=0;
int gValue=0;
int bValue=0;

char *redstr;
char *greenstr;
char *bluestr;

void setup() {
  pinMode(rLedPin, OUTPUT);
  pinMode(gLedPin, OUTPUT);
  pinMode(bLedPin, OUTPUT);

  analogWrite(rLedPin, rValue);
  analogWrite(gLedPin, gValue);
  analogWrite(bLedPin, bValue);

  Serial.begin(9600);
}

void loop() {
  while (Serial.available()) {
    serialByte = Serial.read();
    if (serialByte != '\n') {
      serialBuffer[serialIndex] = serialByte;
      serialIndex++;
    }
    if (serialByte == '\n' or serialIndex == 31) {
      parseSerial();
      serialIndex = 0;
      memset(&serialBuffer, 0, 32);
    }
  }
}

void parseSerial() {
  char *s = serialBuffer;
  redstr = strtok_r(s, " ", &s);
  greenstr = strtok_r(NULL, " ", &s);
  bluestr = strtok_r(NULL, " ", &s);

  rValue = atoi(redstr);
  gValue = atoi(greenstr);
  bValue = atoi(bluestr);

  Serial.print("Red: ");
  Serial.print(rValue);
  Serial.print(" Green: ");
  Serial.print(gValue);
  Serial.print(" Blue: ");
  Serial.println(bValue);

  analogWrite(rLedPin, rValue);
  analogWrite(gLedPin, gValue);
  analogWrite(bLedPin, bValue);
}
