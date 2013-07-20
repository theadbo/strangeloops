// a bunch of intializations
int s0, s1, s2, flx = 0;
int png0, png1, ping2, flex = 0;
boolean s0_ison, s1_ison, s2_ison, flx_ison = false;
String s0Str, s1Str, s2Str, flexStr = "";
int s0_dflt, s1_dflt, s2_dflt = 300;
int s0_crnt, s1_crnt, s2_crnt = 0; //tracks the current 

void setup()
{
  Serial.begin(9600);
  randomSeed(micros());
}

void loop() {
  
  // for pings, if off see if on again
  if (!s0_ison){
    if (random(0,100)>=50) s0_ison=true;
  }
  if (!s1_ison){
    if (random(0,100)<=50) s1_ison=true;
  }
  if (!s2_ison){
    if (random(0,100)>=50) s2_ison=true;
  }
  
  if (s0_ison){
    s0Str = "0:";
    if (s0_crnt < 270){
      if (random(0,100)<30 && s0_crnt > 20){
        s0_crnt = s0-20;
      } else{
        s0_crnt = s0+20;
      }
    }
    s0Str = s0Str + s0_crnt;
    if (random(0,100)<=12){
      s0_ison = false;
      s0 = s0_dflt;
    }
  } else s0Str = s0Str + s0_dflt;
  
  if (s1_ison){
    s1Str = "1:";
    if (s1_crnt < 270){
      if (random(0,100)<30 && s1_crnt > 20){
        s1_crnt = s1-20;
      } else{
        s1_crnt = s1+20;
      }
    }
    s1Str = s1Str + s1_crnt;
    if (random(0,100)<=12){
      s1_ison = false;
      s1 = s1_dflt;
    }
  } else s1Str = s1Str + s1_dflt;
  
  if (s2_ison){
    s2Str = "2:";
    if (s2_crnt < 270){
      if (random(0,100)<30 && s2_crnt > 20){
        s2_crnt = s2-20;
      } else{
        s2_crnt = s2+20;
      }
    }
    s2Str = s2Str + s2_crnt;
    if (random(0,100)<=12){
      s2_ison = false;
      s2 = s2_dflt;
    }
  } else s2Str = s2Str + s2_dflt;
  
  Serial.println(s0Str);
  delay(5);
  Serial.println(s1Str);
  delay(5);
  Serial.println(s2Str);
  // 
  delay(500);
}
