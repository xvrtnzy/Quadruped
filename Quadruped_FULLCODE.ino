#include <Servo.h>

int TrigPin = A0;
int EcPin = A1;
const unsigned long TrigCycle = 50000;
const unsigned long TrigSig = 10;
const unsigned long MaxEc = 25000;
const unsigned long MinEc = 125;
unsigned long TrigTimer = 0;
int TrigVal = LOW;
unsigned long EcInterval;
unsigned long distance;

unsigned long distanceRIGHT;
unsigned long distanceLEFT;
const unsigned long MINdistance = 30;
const unsigned long CRITdistance = 15;
 
Servo R_BACK_1;
Servo R_FRONT_1;
Servo L_FRONT_1;
Servo L_BACK_1;
Servo R_BACK_2;
Servo R_FRONT_2;
Servo L_FRONT_2;
Servo L_BACK_2;
Servo R_BACK_3;
Servo R_FRONT_3;
Servo L_FRONT_3;
Servo L_BACK_3;

int R_BACK_1_angle;
int R_FRONT_1_angle;
int L_FRONT_1_angle;
int L_BACK_1_angle;
int R_BACK_2_angle;
int R_FRONT_2_angle;
int L_FRONT_2_angle;
int L_BACK_2_angle;
int R_BACK_3_angle;
int R_FRONT_3_angle;
int L_FRONT_3_angle;
int L_BACK_3_angle;

int R_BACK_1_PREVangle;
int R_FRONT_1_PREVangle;
int L_BACK_1_PREVangle;
int L_FRONT_1_PREVangle;

int L_FRONT_1_MAXangle = 98;
int L_FRONT_1_MINangle = 45;
int R_BACK_1_MAXangle = 98;
int R_BACK_1_MINangle = 45;

unsigned long prevmicros;

const unsigned long stepBwdSpeed = 21000;
const unsigned long stepFwdSpeed = 3000;
const unsigned long turnBwdSpeed = 18000;
const unsigned long turnFwdSpeed = 6000;
const unsigned long stallSpeed = 30000;

int R_BACK_STALLvar;
int R_FRONT_STALLvar;
int L_BACK_STALLvar;
int L_FRONT_STALLvar;

int R_BACK_stepUP;
int R_FRONT_stepUP;
int L_BACK_stepUP;
int L_FRONT_stepUP;

int turnRightVAR;
int turnLeftVAR;
int stallVAR;
int ultrasoundSWITCH;
int randomTURN;
int CRITvar;

unsigned long prevmillis;

void setup() {
  pinMode(TrigPin,OUTPUT);
  pinMode(EcPin,INPUT);
  
  prevmillis = millis();
  while(millis() - prevmillis < 5000) {
    Ultrasound();
  }
  
  R_BACK_1.attach(2,520,2470);
  R_FRONT_1.attach(3,350,2390);
  L_FRONT_1.attach(4,580,2500);
  L_BACK_1.attach(5,400,2500);
  R_BACK_2.attach(6,400,2500);
  R_FRONT_2.attach(7,350,2200);
  L_FRONT_2.attach(8,600,2650);
  L_BACK_2.attach(13,350,2500);
  R_BACK_3.attach(12,500,2450);
  R_FRONT_3.attach(11,500,2500);
  L_FRONT_3.attach(10,400,2450);
  L_BACK_3.attach(9,500,2600);
  
  R_BACK_1_angle = 90;
  R_FRONT_1_angle = 90;
  L_FRONT_1_angle = 90;
  L_BACK_1_angle = 90;
  R_BACK_2_angle = 59;
  R_FRONT_2_angle = 122;
  L_FRONT_2_angle = 59;
  L_BACK_2_angle = 122;
  R_BACK_3_angle = 83;
  R_FRONT_3_angle = 98;
  L_FRONT_3_angle = 83;
  L_BACK_3_angle = 98;
  
  R_BACK_1.write(R_BACK_1_angle);
  R_FRONT_1.write(R_FRONT_1_angle);
  L_FRONT_1.write(L_FRONT_1_angle);
  L_BACK_1.write(L_BACK_1_angle);
  R_BACK_2.write(R_BACK_2_angle);
  R_FRONT_2.write(R_FRONT_2_angle);
  L_FRONT_2.write(L_FRONT_2_angle);
  L_BACK_2.write(L_BACK_2_angle);
  R_BACK_3.write(R_BACK_3_angle);
  R_FRONT_3.write(R_FRONT_3_angle);
  L_FRONT_3.write(L_FRONT_3_angle);
  L_BACK_3.write(L_BACK_3_angle);
  
  prevmillis = millis();
  while(millis() - prevmillis < 2000) {
    Ultrasound();
  }
}

void loop() {
  randomTURN = random(1,4);
  
  if(distance > MINdistance) {
    ultrasoundSWITCH = 0;
    walkFWD();
    while((R_BACK_1_angle == 98 || L_BACK_1_angle == 83) && ultrasoundSWITCH == 0) {
      Ultrasound();
    }
  }
  
  else if(distance <= MINdistance && distance > CRITdistance) {
    resetPREVangle();
    ultrasoundSWITCH = 1;
    stallVAR = 0;
    turnRightVAR = 0;
    turnLeftVAR = 0;
    
    rotate();
  }
  
  else if(distance <= CRITdistance) {
    CRITvar = 1;
    switch(CRITvar) {
      case 1 :
        resetPREVangle();
        prevmillis = millis();
        while(millis() - prevmillis < 5000) {
          walkBWD();
          CRITvar = 2;
        }
        
      case 2 :
        resetPREVangle();
        ultrasoundSWITCH = 1;
        stallVAR = 0;
        turnRightVAR = 0;
        turnLeftVAR = 0;
        
        rotate();
        break;
    }
  }
}

void walkFWD() {
  RightBackFWD();
  RightFrontFWD();
  LeftBackFWD();
  LeftFrontFWD();
}

void walkBWD() {
  LeftFrontBWD();
  LeftBackBWD();
  RightFrontBWD();
  RightBackBWD();
}

void turnLEFT() {
  RightBackTRNL();
  RightFrontTRNL();
  LeftFrontTRNL();
  LeftBackTRNL();  
}

void turnRIGHT() {
  LeftBackTRNR();
  LeftFrontTRNR();
  RightFrontTRNR();
  RightBackTRNR();
}

/* ---------------------------------- ROTATE -------------------------------------------------*/

void rotate() {
  while(randomTURN == 1 || randomTURN == 2) {
    while(stallVAR != 4) {
      stall();
    }
    while(turnRightVAR != 3) {
      turnRIGHT();
    }
    while(ultrasoundSWITCH == 1) {
      Ultrasound();
      distanceRIGHT = distance;
    }
    while(turnLeftVAR != 6) {
      turnLEFT();
    }
    while(ultrasoundSWITCH == 2) {
      Ultrasound();
      distanceLEFT = distance;
    }
    turnRightVAR = 0;
    turnLeftVAR = 0;
    if(distanceRIGHT > distanceLEFT && distanceRIGHT > MINdistance) {
      resetPREVangle();
      while(turnRightVAR != 6) {
        turnRIGHT();
        if(turnRightVAR == 6) {
          randomTURN = 0;
        }
      }
    }
    else if(distanceLEFT >= distanceRIGHT && distanceLEFT > MINdistance) {
      resetPREVangle();
      randomTURN = 0;
    }
    else {
      resetPREVangle();
      while(turnLeftVAR != 3) {
        turnLEFT();
        if(turnLeftVAR == 3) {
          randomTURN = 0;
        }
      }
    }
  }
  
  while(randomTURN == 3 || randomTURN == 4) {
    while(stallVAR != 4) {
      stall();
    }
    while(turnLeftVAR != 3) {
      turnLEFT();
    }
    while(ultrasoundSWITCH == 0) {
      Ultrasound();
      distanceLEFT = distance;
    }
    while(turnRightVAR != 6) {
      turnRIGHT();
    }
    while(ultrasoundSWITCH == 1) {
      Ultrasound();
      distanceRIGHT = distance;
    }
    turnRightVAR = 0;
    turnLeftVAR = 0;
    if(distanceLEFT > distanceRIGHT && distanceLEFT > MINdistance) {
      resetPREVangle();
      while(turnLeftVAR != 6) {
        turnLEFT();
        if(turnLeftVAR == 6) {
          randomTURN = 0;
        }
      }
    }
    else if(distanceRIGHT >= distanceLEFT && distanceRIGHT > MINdistance) {
      resetPREVangle();
      randomTURN = 0;
    }
    else {
      resetPREVangle();
      while(turnRightVAR != 3) {
        turnRIGHT();
        if(turnRightVAR == 3) {
          randomTURN = 0;
        }
      }
    }
  }
}

/* ---------------------------------- END OF ROTATE -------------------------------------------------*/

/* ------------------------------- ULTRASOUND READING ----------------------------------------------*/

void Ultrasound() {
  if (TrigVal == LOW && micros()- TrigTimer > TrigCycle) {
    TrigVal = HIGH;
    digitalWrite (TrigPin,TrigVal);
    TrigTimer = micros();
  }
  else if (TrigVal == HIGH && micros()- TrigTimer > TrigSig) {
    TrigVal = LOW;
    digitalWrite (TrigPin,TrigVal);
  }
  else if (TrigVal == LOW) {
    EcInterval = pulseIn(EcPin,HIGH,TrigCycle);
    if (EcInterval > 0 && EcInterval <= MaxEc) {
      distance = map (EcInterval, MinEc, MaxEc, 2, 470);
      ultrasoundSWITCH = ultrasoundSWITCH + 1;
    }
    else if (EcInterval > MaxEc) {
      distance = 470;
      ultrasoundSWITCH = ultrasoundSWITCH + 1;
    }
  }
}

/* ------------------------------- END OF ULTRASOUND READING ----------------------------------------------*/

/* ------------------------------- RESET PREVangle ----------------------------------------------*/

void resetPREVangle() {
  R_BACK_1_PREVangle = 0;
  R_FRONT_1_PREVangle = 0;
  L_BACK_1_PREVangle = 0;
  L_FRONT_1_PREVangle = 0;
}

/* ------------------------------- END OF RESET PREVangle ----------------------------------------------*/

/* ------------------------------- STALL POSITION ----------------------------------------------*/

void stall() {
  /* 
  R_BACK_1_angle,  min.45,  std.90,  up.79,  switch.63/80,   max.98
  R_BACK_2_angle,  min.49,  std.59,  up.19,  switch.56/62,   max.69
  R_BACK_3_angle,  min.52,  std.83,  up.51,  switch.70/88,   max.106
  R_FRONT_1_angle, min.135, std.90,  up.102, switch.117/101, max.83
  R_FRONT_2_angle, min.132, std.122, up.161, switch.125/118, max.111
  R_FRONT_3_angle, min.129, std.98,  up.130, switch.111/93,  max.75
  L_FRONT_1_angle, min.45,  std.90,  up.79,  switch.63/80,   max.98
  L_FRONT_2_angle, min.49,  std.59,  up.19,  switch.56/62,   max.69
  L_FRONT_3_angle, min.52,  std.83,  up.51,  switch.70/88,   max.106
  L_BACK_1_angle,  min.135, std.90,  up.102, switch.117/101, max.83
  L_BACK_2_angle,  min.132, std.122, up.161, switch.125/118, max.111
  L_BACK_3_angle,  min.129, std.98,  up.130, switch.111/93,  max.75
  */
  
  //---------------------------------------------------------------------------------
    
  if(R_BACK_stepUP == 1 || L_FRONT_STALLvar == 1) {
    int R_BACK_1_CURangle = R_BACK_1_angle;
    int R_BACK_2_CURangle = R_BACK_2_angle;
    int R_BACK_3_CURangle = R_BACK_3_angle;
    while(R_BACK_1_angle != 90) {
      if(R_BACK_1_angle < 90) {
        int R_BACK_1_MIDangle = ((90 - R_BACK_1_CURangle) / 2) + R_BACK_1_CURangle;
        R_BACK_1_angle = R_BACK_1_angle + 1;
        R_BACK_1.write(R_BACK_1_angle);
        if(R_BACK_1_angle < R_BACK_1_MIDangle) {
          R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_CURangle, R_BACK_1_MIDangle, R_BACK_2_CURangle, 19);
          R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_CURangle, R_BACK_1_MIDangle, R_BACK_3_CURangle, 51);
          R_BACK_2.write(R_BACK_2_angle);
          R_BACK_3.write(R_BACK_3_angle);
        }
        else if(R_BACK_1_angle >= R_BACK_1_MIDangle) {
          R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_MIDangle, 90, 19, 59);
          R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_MIDangle, 90, 51, 83);
          R_BACK_2.write(R_BACK_2_angle);
          R_BACK_3.write(R_BACK_3_angle);
        }          
        prevmicros = micros();
        while(micros() - prevmicros <= stallSpeed) {
        }        
      }
      else if(R_BACK_1_angle > 90) {
        int R_BACK_1_MIDangle = ((R_BACK_1_CURangle - 90) / 2) + 90;
        R_BACK_1_angle = R_BACK_1_angle - 1;
        R_BACK_1.write(R_BACK_1_angle);
        if(R_BACK_1_angle > R_BACK_1_MIDangle) {
          R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_CURangle, R_BACK_1_MIDangle, R_BACK_2_CURangle, 19);
          R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_CURangle, R_BACK_1_MIDangle, R_BACK_3_CURangle, 51);
          R_BACK_2.write(R_BACK_2_angle);
          R_BACK_3.write(R_BACK_3_angle);
        }
        else if(R_BACK_1_angle <= R_BACK_1_MIDangle) {
          R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_MIDangle, 90, 19, 59);
          R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_MIDangle, 90, 51, 83);
          R_BACK_2.write(R_BACK_2_angle);
          R_BACK_3.write(R_BACK_3_angle);
        }          
        prevmicros = micros();
        while(micros() - prevmicros <= stallSpeed) {
        }
      }
    }
    if(R_BACK_1_angle == 90) {
      R_BACK_2_angle = 59;
      R_BACK_3_angle = 83;
      R_BACK_2.write(R_BACK_2_angle);
      R_BACK_3.write(R_BACK_3_angle);
      R_BACK_1_PREVangle = 0;
      R_BACK_STALLvar = 1;
      L_FRONT_STALLvar = 0;
      stallVAR = stallVAR + 1;
    }
  }
  
  //---------------------------------------------------------------------------------
  
  else if(R_FRONT_stepUP == 1 || R_BACK_STALLvar == 1) {
    int R_FRONT_1_CURangle = R_FRONT_1_angle;
    int R_FRONT_2_CURangle = R_FRONT_2_angle;
    int R_FRONT_3_CURangle = R_FRONT_3_angle;
    while(R_FRONT_1_angle != 90) {
      if(R_FRONT_1_angle > 90) {
        int R_FRONT_1_MIDangle = ((R_FRONT_1_CURangle - 90) / 2) + 90;
        R_FRONT_1_angle = R_FRONT_1_angle - 1;
        R_FRONT_1.write(R_FRONT_1_angle);
        if(R_FRONT_1_angle > R_FRONT_1_MIDangle) {
          R_FRONT_2_angle = map(R_FRONT_1_angle, R_FRONT_1_CURangle, R_FRONT_1_MIDangle, R_FRONT_2_CURangle, 161);
          R_FRONT_3_angle = map(R_FRONT_1_angle, R_FRONT_1_CURangle, R_FRONT_1_MIDangle, R_FRONT_3_CURangle, 130);
          R_FRONT_2.write(R_FRONT_2_angle);
          R_FRONT_3.write(R_FRONT_3_angle);
        }
        else if(R_FRONT_1_angle <= R_FRONT_1_MIDangle) {
          R_FRONT_2_angle = map(R_FRONT_1_angle, R_FRONT_1_MIDangle, 90, 111, 122);
          R_FRONT_3_angle = map(R_FRONT_1_angle, R_FRONT_1_MIDangle, 90, 75, 98);
          R_FRONT_2.write(R_FRONT_2_angle);
          R_FRONT_3.write(R_FRONT_3_angle);
        }          
        prevmicros = micros();
        while(micros() - prevmicros <= stallSpeed) {
        }        
      }
      else if(R_FRONT_1_angle < 90) {
        int R_FRONT_1_MIDangle = ((90 - R_FRONT_1_CURangle) / 2) + R_FRONT_1_CURangle;
        R_FRONT_1_angle = R_FRONT_1_angle + 1;
        R_FRONT_1.write(R_FRONT_1_angle);
        if(R_FRONT_1_angle < R_FRONT_1_MIDangle) {
          R_FRONT_2_angle = map(R_FRONT_1_angle, R_FRONT_1_CURangle, R_FRONT_1_MIDangle, R_FRONT_2_CURangle, 161);
          R_FRONT_3_angle = map(R_FRONT_1_angle, R_FRONT_1_CURangle, R_FRONT_1_MIDangle, R_FRONT_3_CURangle, 130);
          R_FRONT_2.write(R_FRONT_2_angle);
          R_FRONT_3.write(R_FRONT_3_angle);
        }
        else if(R_FRONT_1_angle >= R_FRONT_1_MIDangle) {
          R_FRONT_2_angle = map(R_FRONT_1_angle, R_FRONT_1_MIDangle, 90, 111, 122);
          R_FRONT_3_angle = map(R_FRONT_1_angle, R_FRONT_1_MIDangle, 90, 75, 98);
          R_FRONT_2.write(R_FRONT_2_angle);
          R_FRONT_3.write(R_FRONT_3_angle);
        }          
        prevmicros = micros();
        while(micros() - prevmicros <= stallSpeed) {
        }
      }
    }
    if(R_FRONT_1_angle == 90) {
      R_FRONT_2_angle = 122;
      R_FRONT_3_angle = 98;
      R_FRONT_2.write(R_FRONT_2_angle);
      R_FRONT_3.write(R_FRONT_3_angle);
      R_FRONT_1_PREVangle = 0;
      R_FRONT_STALLvar = 1;
      R_BACK_STALLvar = 0;
      stallVAR = stallVAR + 1;
    }
  }
  
  //---------------------------------------------------------------------------------
  
  else if(L_BACK_stepUP == 1 || R_FRONT_STALLvar == 1) {
    int L_BACK_1_CURangle = L_BACK_1_angle;
    int L_BACK_2_CURangle = L_BACK_2_angle;
    int L_BACK_3_CURangle = L_BACK_3_angle;
    while(L_BACK_1_angle != 90) {
      if(L_BACK_1_angle > 90) {
        int L_BACK_1_MIDangle = ((L_BACK_1_CURangle - 90) / 2) + 90;
        L_BACK_1_angle = L_BACK_1_angle - 1;
        L_BACK_1.write(L_BACK_1_angle);
        if(L_BACK_1_angle > L_BACK_1_MIDangle) {
          L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_CURangle, L_BACK_1_MIDangle, L_BACK_2_CURangle, 161);
          L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_CURangle, L_BACK_1_MIDangle, L_BACK_3_CURangle, 130);
          L_BACK_2.write(L_BACK_2_angle);
          L_BACK_3.write(L_BACK_3_angle);
        }
        else if(L_BACK_1_angle <= L_BACK_1_MIDangle) {
          L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_MIDangle, 90, 111, 122);
          L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_MIDangle, 90, 75, 98);
          L_BACK_2.write(L_BACK_2_angle);
          L_BACK_3.write(L_BACK_3_angle);
        }          
        prevmicros = micros();
        while(micros() - prevmicros <= stallSpeed) {
        }        
      }
      else if(L_BACK_1_angle < 90) {
        int L_BACK_1_MIDangle = ((90 - L_BACK_1_CURangle) / 2) + L_BACK_1_CURangle;
        L_BACK_1_angle = L_BACK_1_angle + 1;
        L_BACK_1.write(L_BACK_1_angle);
        if(L_BACK_1_angle < L_BACK_1_MIDangle) {
          L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_CURangle, L_BACK_1_MIDangle, L_BACK_2_CURangle, 161);
          L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_CURangle, L_BACK_1_MIDangle, L_BACK_3_CURangle, 130);
          L_BACK_2.write(L_BACK_2_angle);
          L_BACK_3.write(L_BACK_3_angle);
        }
        else if(L_BACK_1_angle >= L_BACK_1_MIDangle) {
          L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_MIDangle, 90, 111, 122);
          L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_MIDangle, 90, 75, 98);
          L_BACK_2.write(L_BACK_2_angle);
          L_BACK_3.write(L_BACK_3_angle);
        }          
        prevmicros = micros();
        while(micros() - prevmicros <= stallSpeed) {
        }
      }
    }
    if(L_BACK_1_angle == 90) {
      L_BACK_2_angle = 122;
      L_BACK_3_angle = 98;
      L_BACK_2.write(L_BACK_2_angle);
      L_BACK_3.write(L_BACK_3_angle);
      L_BACK_1_PREVangle = 0;
      L_BACK_STALLvar = 1;
      R_FRONT_STALLvar = 0;
      stallVAR = stallVAR + 1;
    }
  }
  
  //---------------------------------------------------------------------------------
  
  else if(L_FRONT_stepUP == 1 || L_BACK_STALLvar == 1) {
    int L_FRONT_1_CURangle = L_FRONT_1_angle;
    int L_FRONT_2_CURangle = L_FRONT_2_angle;
    int L_FRONT_3_CURangle = L_FRONT_3_angle;
    while(L_FRONT_1_angle != 90) {
      if(L_FRONT_1_angle < 90) {
        int L_FRONT_1_MIDangle = ((90 - L_FRONT_1_CURangle) / 2) + L_FRONT_1_CURangle;
        L_FRONT_1_angle = L_FRONT_1_angle + 1;
        L_FRONT_1.write(L_FRONT_1_angle);
        if(L_FRONT_1_angle < L_FRONT_1_MIDangle) {
          L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_CURangle, L_FRONT_1_MIDangle, L_FRONT_2_CURangle, 19);
          L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_CURangle, L_FRONT_1_MIDangle, L_FRONT_3_CURangle, 51);
          L_FRONT_2.write(L_FRONT_2_angle);
          L_FRONT_3.write(L_FRONT_3_angle);
        }
        else if(L_FRONT_1_angle >= L_FRONT_1_MIDangle) {
          L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_MIDangle, 90, 19, 59);
          L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_MIDangle, 90, 51, 83);
          L_FRONT_2.write(L_FRONT_2_angle);
          L_FRONT_3.write(L_FRONT_3_angle);
        }          
        prevmicros = micros();
        while(micros() - prevmicros <= stallSpeed) {
        }        
      }
      else if(L_FRONT_1_angle > 90) {
        int L_FRONT_1_MIDangle = ((L_FRONT_1_CURangle - 90) / 2) + 90;
        L_FRONT_1_angle = L_FRONT_1_angle - 1;
        L_FRONT_1.write(L_FRONT_1_angle);
        if(L_FRONT_1_angle > L_FRONT_1_MIDangle) {
          L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_CURangle, L_FRONT_1_MIDangle, L_FRONT_2_CURangle, 19);
          L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_CURangle, L_FRONT_1_MIDangle, L_FRONT_3_CURangle, 51);
          L_FRONT_2.write(L_FRONT_2_angle);
          L_FRONT_3.write(L_FRONT_3_angle);
        }
        else if(L_FRONT_1_angle <= L_FRONT_1_MIDangle) {
          L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_MIDangle, 90, 19, 59);
          L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_MIDangle, 90, 51, 83);
          L_FRONT_2.write(L_FRONT_2_angle);
          L_FRONT_3.write(L_FRONT_3_angle);
        }          
        prevmicros = micros();
        while(micros() - prevmicros <= stallSpeed) {
        }
      }
    }
    if(L_FRONT_1_angle == 90) {
      L_FRONT_2_angle = 59;
      L_FRONT_3_angle = 83;
      L_FRONT_2.write(L_FRONT_2_angle);
      L_FRONT_3.write(L_FRONT_3_angle);
      L_FRONT_1_PREVangle = 0;
      L_FRONT_STALLvar = 1;
      L_BACK_STALLvar = 0;
      stallVAR = stallVAR + 1;
    }
  }
  
  //---------------------------------------------------------------------------------
  
  else {
    L_FRONT_STALLvar = 1;
  }
}

/* ------------------------------- END OF STALL POSITION ----------------------------------------------*/

/* ------------------------------- TURN LEFT ----------------------------------------------*/

void RightBackTRNL() {
  
  /*
  R_BACK_1_angle,    90  ^ 46  > 50  > 75  > 90  // up. 68 
  R_BACK_2_angle,    58  ^ 63  > ??  > ??  > 58  // up.19
  R_BACK_3_angle,    83  ^ 93  > ??  > ??  > 83  // up.80
  
  R_FRONT_1_angle,   90  > 105 ^ 50  > ??  > 90  // up. 78
  R_FRONT_2_angle,   122 > 122 ^ 120 > ??  > 122 // up.161
  R_FRONT_3_angle,   98  > 98  ^ 93  > ??  > 98  // up.100
  
  L_FRONT_1_angle,   90  > 105 > 119 ^ 75  > 90  // up. 112
  L_FRONT_2_angle,   58  > 58  > 60  ^ 59  > 58  // up.19
  L_FRONT_3_angle,   83  > 83  > 87  ^ 84  > 83  // up.80
  
  L_BACK_1_angle,    90  > 105 > ??  > 133 ^ 90  // up. 97
  L_BACK_2_angle,    122 > 122 > ??  > 117 ^ 122 // up.161
  L_BACK_3_angle,    98  > 98  > ??  > 86  ^ 98  // up.100
  */
  
  int R_BACK_1_MAXangle = 90;
  int R_BACK_1_MINangle = 46;
  int R_BACK_1_UPangle = R_BACK_1_MINangle + ((R_BACK_1_MAXangle - R_BACK_1_MINangle) / 2);
  int R_BACK_2_MAXangle = 58;
  int R_BACK_2_MINangle = 63;
  int R_BACK_2_UPangle = 19;
  int R_BACK_3_MAXangle = 83;
  int R_BACK_3_MINangle = 93;
  int R_BACK_3_UPangle = 80;
  
  if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle <= R_BACK_1_MAXangle) {
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = R_BACK_1_angle + 1;
    
    R_BACK_1.write(R_BACK_1_angle);
    R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_MINangle, R_BACK_1_MAXangle, R_BACK_2_MINangle, R_BACK_2_MAXangle);
    R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_MINangle, R_BACK_1_MAXangle, R_BACK_3_MINangle, R_BACK_3_MAXangle);
    R_BACK_2.write(R_BACK_2_angle);
    R_BACK_3.write(R_BACK_3_angle);    
    
    prevmicros = micros();
    while(micros() - prevmicros <= turnBwdSpeed) {
    }
  }
  
  else if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle > R_BACK_1_MAXangle){
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = R_BACK_1_MAXangle - 1;
    turnLeftVAR = turnLeftVAR + 1;
  }
  
  else if(R_BACK_1_angle < R_BACK_1_PREVangle && R_BACK_1_angle >= R_BACK_1_MINangle){
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = R_BACK_1_angle - 1;
    
    R_BACK_1.write(R_BACK_1_angle);
    
    if(R_BACK_1_angle <= R_BACK_1_UPangle) {
      R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_UPangle, R_BACK_1_MINangle, R_BACK_2_UPangle, R_BACK_2_MINangle);
      R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_UPangle, R_BACK_1_MINangle, R_BACK_3_UPangle, R_BACK_3_MINangle);
      R_BACK_2.write(R_BACK_2_angle);
      R_BACK_3.write(R_BACK_3_angle);
    }
    else if(R_BACK_1_angle > R_BACK_1_UPangle) {
      R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_MAXangle, R_BACK_1_UPangle, R_BACK_2_MAXangle, R_BACK_2_UPangle);
      R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_MAXangle, R_BACK_1_UPangle, R_BACK_3_MAXangle, R_BACK_3_UPangle);
      R_BACK_2.write(R_BACK_2_angle);
      R_BACK_3.write(R_BACK_3_angle);
    }
    
    prevmicros = micros();
    while(micros() - prevmicros <= turnFwdSpeed) {
    }
  }
  
  else if(R_BACK_1_angle < R_BACK_1_PREVangle && R_BACK_1_angle < R_BACK_1_MINangle){
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = R_BACK_1_MINangle + 1;
  }
}

void RightFrontTRNL() {
  
  /*
  R_BACK_1_angle,    90  ^ 46  > 50  > 75  > 90  // up. 68 
  R_BACK_2_angle,    58  ^ 63  > ??  > ??  > 58  // up.19
  R_BACK_3_angle,    83  ^ 93  > ??  > ??  > 83  // up.80
  
  R_FRONT_1_angle,   90  > 105 ^ 50  > ??  > 90  // up. 78
  R_FRONT_2_angle,   122 > 122 ^ 120 > ??  > 122 // up.161
  R_FRONT_3_angle,   98  > 98  ^ 93  > ??  > 98  // up.100
  
  L_FRONT_1_angle,   90  > 105 > 119 ^ 75  > 90  // up. 112
  L_FRONT_2_angle,   58  > 58  > 60  ^ 59  > 58  // up.19
  L_FRONT_3_angle,   83  > 83  > 87  ^ 84  > 83  // up.80
  
  L_BACK_1_angle,    90  > 105 > ??  > 133 ^ 90  // up. 97
  L_BACK_2_angle,    122 > 122 > ??  > 117 ^ 122 // up.161
  L_BACK_3_angle,    98  > 98  > ??  > 86  ^ 98  // up.100
  */
  
  int R_FRONT_1_UPangle = 105 - ((105 - 50) / 2) ;
  
  if(R_BACK_1_angle < R_BACK_1_PREVangle) {
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(R_BACK_1_angle, 90, 46, 90, 105);
    R_FRONT_2_angle = map(R_BACK_1_angle, 90, 46, 122, 122);
    R_FRONT_3_angle = map(R_BACK_1_angle, 90, 46, 98, 98);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }
  
  else if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle <= 50) {
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(R_BACK_1_angle, 46, 50, 105, 50);
    R_FRONT_1.write(R_FRONT_1_angle);
    if(R_FRONT_1_angle >= R_FRONT_1_UPangle) {
      R_FRONT_2_angle = map(R_FRONT_1_angle, 105, R_FRONT_1_UPangle, 122, 161);
      R_FRONT_3_angle = map(R_FRONT_1_angle, 105, R_FRONT_1_UPangle, 98, 100);
      R_FRONT_2.write(R_FRONT_2_angle);
      R_FRONT_3.write(R_FRONT_3_angle);
    }
    else if(R_FRONT_1_angle < R_FRONT_1_UPangle) {
      R_FRONT_2_angle = map(R_FRONT_1_angle, R_FRONT_1_UPangle, 50, 161, 120);
      R_FRONT_3_angle = map(R_FRONT_1_angle, R_FRONT_1_UPangle, 50, 100, 93);
      R_FRONT_2.write(R_FRONT_2_angle);
      R_FRONT_3.write(R_FRONT_3_angle);
    }
  }
  
  else if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle > 50) {
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(R_BACK_1_angle, 50, 90, 50, 90);
    R_FRONT_2_angle = map(R_BACK_1_angle, 50, 90, 120, 122);
    R_FRONT_3_angle = map(R_BACK_1_angle, 50, 90, 93, 98);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }  
}

void LeftFrontTRNL() {
  
  /*
  R_BACK_1_angle,    90  ^ 46  > 50  > 75  > 90  // up. 68 
  R_BACK_2_angle,    58  ^ 63  > ??  > ??  > 58  // up.19
  R_BACK_3_angle,    83  ^ 93  > ??  > ??  > 83  // up.80
  
  R_FRONT_1_angle,   90  > 105 ^ 50  > ??  > 90  // up. 78
  R_FRONT_2_angle,   122 > 122 ^ 120 > ??  > 122 // up.161
  R_FRONT_3_angle,   98  > 98  ^ 93  > ??  > 98  // up.100
  
  L_FRONT_1_angle,   90  > 105 > 119 ^ 75  > 90  // up. 112
  L_FRONT_2_angle,   58  > 58  > 60  ^ 59  > 58  // up.19
  L_FRONT_3_angle,   83  > 83  > 87  ^ 84  > 83  // up.80
  
  L_BACK_1_angle,    90  > 105 > ??  > 133 ^ 90  // up. 97
  L_BACK_2_angle,    122 > 122 > ??  > 117 ^ 122 // up.161
  L_BACK_3_angle,    98  > 98  > ??  > 86  ^ 98  // up.100
  */
  
  int L_FRONT_1_UPangle = 75 + ((119 - 75) / 2);
  
  if(R_BACK_1_angle < R_BACK_1_PREVangle) {
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(R_BACK_1_angle, 90, 46, 90, 105);
    L_FRONT_2_angle = map(R_BACK_1_angle, 90, 46, 58, 58);
    L_FRONT_3_angle = map(R_BACK_1_angle, 90, 46, 83, 83);
    L_FRONT_1.write(L_FRONT_1_angle);
    L_FRONT_2.write(L_FRONT_2_angle);
    L_FRONT_3.write(L_FRONT_3_angle);
  }
  else if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle < 50) {
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(R_BACK_1_angle, 46, 50, 105, 119);
    L_FRONT_2_angle = map(R_BACK_1_angle, 46, 50, 58, 60);
    L_FRONT_3_angle = map(R_BACK_1_angle, 46, 50, 83, 87);
    L_FRONT_1.write(L_FRONT_1_angle);
    L_FRONT_2.write(L_FRONT_2_angle);
    L_FRONT_3.write(L_FRONT_3_angle);
  }
  else if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle >= 50 && R_BACK_1_angle <= 75) {
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(R_BACK_1_angle, 50, 75, 119, 75);
    L_FRONT_1.write(L_FRONT_1_angle);
    if(L_FRONT_1_angle >= L_FRONT_1_UPangle) {
      L_FRONT_2_angle = map(L_FRONT_1_angle, 119, L_FRONT_1_UPangle, 60, 19);
      L_FRONT_3_angle = map(L_FRONT_1_angle, 119, L_FRONT_1_UPangle, 87, 80);
      L_FRONT_2.write(L_FRONT_2_angle);
      L_FRONT_3.write(L_FRONT_3_angle);
    }
    else if(L_FRONT_1_angle < L_FRONT_1_UPangle) {
      L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_UPangle, 75, 19, 59);
      L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_UPangle, 75, 80, 84);
      L_FRONT_2.write(L_FRONT_2_angle);
      L_FRONT_3.write(L_FRONT_3_angle);
    }
  }
  else if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle > 75) {
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(R_BACK_1_angle, 75, 90, 75, 90);
    L_FRONT_2_angle = map(R_BACK_1_angle, 75, 90, 59, 58);
    L_FRONT_3_angle = map(R_BACK_1_angle, 75, 90, 84, 83);
    L_FRONT_1.write(L_FRONT_1_angle);
    L_FRONT_2.write(L_FRONT_2_angle);
    L_FRONT_3.write(L_FRONT_3_angle);
  }
}

void LeftBackTRNL() {
  
  /*
  R_BACK_1_angle,    90  ^ 46  > 50  > 75  > 90  // up. 68 
  R_BACK_2_angle,    58  ^ 63  > ??  > ??  > 58  // up.19
  R_BACK_3_angle,    83  ^ 93  > ??  > ??  > 83  // up.80
  
  R_FRONT_1_angle,   90  > 105 ^ 50  > ??  > 90  // up. 78
  R_FRONT_2_angle,   122 > 122 ^ 120 > ??  > 122 // up.161
  R_FRONT_3_angle,   98  > 98  ^ 93  > ??  > 98  // up.100
  
  L_FRONT_1_angle,   90  > 105 > 119 ^ 75  > 90  // up. 112
  L_FRONT_2_angle,   58  > 58  > 60  ^ 59  > 58  // up.19
  L_FRONT_3_angle,   83  > 83  > 87  ^ 84  > 83  // up.80
  
  L_BACK_1_angle,    90  > 105 > ??  > 133 ^ 90  // up. 97
  L_BACK_2_angle,    122 > 122 > ??  > 117 ^ 122 // up.161
  L_BACK_3_angle,    98  > 98  > ??  > 86  ^ 98  // up.100
  */

  int L_BACK_1_UPangle = 90 + ((133 - 90) / 2);

  if(R_BACK_1_angle < R_BACK_1_PREVangle) {
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(R_BACK_1_angle, 90, 46, 90, 105);
    L_BACK_2_angle = map(R_BACK_1_angle, 90, 46, 122, 122);
    L_BACK_3_angle = map(R_BACK_1_angle, 90, 46, 98, 98);
    L_BACK_1.write(L_BACK_1_angle);
    L_BACK_2.write(L_BACK_2_angle);
    L_BACK_3.write(L_BACK_3_angle);
  }
  else if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle < 75) {
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(R_BACK_1_angle, 46, 75, 105, 133);
    L_BACK_2_angle = map(R_BACK_1_angle, 46, 75, 122, 117);
    L_BACK_3_angle = map(R_BACK_1_angle, 46, 75, 98, 86);
    L_BACK_1.write(L_BACK_1_angle);
    L_BACK_2.write(L_BACK_2_angle);
    L_BACK_3.write(L_BACK_3_angle);
  }
  else if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle >= 75) {
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(R_BACK_1_angle, 75, 90, 133, 90);
    L_BACK_1.write(L_BACK_1_angle);
    if(L_BACK_1_angle >= L_BACK_1_UPangle) {
      L_BACK_2_angle = map(L_BACK_1_angle, 133, L_BACK_1_UPangle, 117, 161);
      L_BACK_3_angle = map(L_BACK_1_angle, 133, L_BACK_1_UPangle, 86, 100);
      L_BACK_2.write(L_BACK_2_angle);
      L_BACK_3.write(L_BACK_3_angle);
    }
    else if(L_BACK_1_angle < L_BACK_1_UPangle) {
      L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_UPangle, 90, 161, 122);
      L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_UPangle, 90, 100, 98);
      L_BACK_2.write(L_BACK_2_angle);
      L_BACK_3.write(L_BACK_3_angle);
    }
  }
}

/* ------------------------------- END OF TURN LEFT ----------------------------------------------*/

/* ------------------------------- TURN RIGHT ----------------------------------------------*/

void LeftBackTRNR() {
  
  /*
  L_BACK_1_angle,    90  ^ 134 < 130 < 105 < 90  // up. 68 
  L_BACK_2_angle,    122 ^ 117 < ??  < ??  < 122 // up.161
  L_BACK_3_angle,    98  ^ 87  < ??  < ??  < 98  // up.100
  
  L_FRONT_1_angle,   90  < 75  ^ 130 < ??  < 90  // up. 78
  L_FRONT_2_angle,   58  < 58  ^ 60  < ??  < 58  // up.19
  L_FRONT_3_angle,   83  < 83  ^ 87  < ??  < 83  // up.80
  
  R_FRONT_1_angle,   90  < 75  < 61  ^ 105 < 90  // up. 112
  R_FRONT_2_angle,   122 < 122 < 120 ^ 121 < 122 // up.161
  R_FRONT_3_angle,   98  < 98  < 93  ^ 96  < 98  // up.100
  
  R_BACK_1_angle,    90  < 75  < ??  < 46  ^ 90  // up. 97
  R_BACK_2_angle,    58  < 58  < ??  < 63  ^ 58  // up.19
  R_BACK_3_angle,    83  < 83  < ??  < 94  ^ 83  // up.80
  */
  
  int L_BACK_1_MAXangle = 90;
  int L_BACK_1_MINangle = 134;
  int L_BACK_1_UPangle = L_BACK_1_MINangle - ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 2);
  int L_BACK_2_MAXangle = 122;
  int L_BACK_2_MINangle = 117;
  int L_BACK_2_UPangle = 161;
  int L_BACK_3_MAXangle = 98;
  int L_BACK_3_MINangle = 87;
  int L_BACK_3_UPangle = 100;
  
  if(L_BACK_1_angle < L_BACK_1_PREVangle && L_BACK_1_angle >= L_BACK_1_MAXangle) {
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = L_BACK_1_angle - 1;
    
    L_BACK_1.write(L_BACK_1_angle);
    L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_MAXangle, L_BACK_2_MINangle, L_BACK_2_MAXangle);
    L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_MAXangle, L_BACK_3_MINangle, L_BACK_3_MAXangle);
    L_BACK_2.write(L_BACK_2_angle);
    L_BACK_3.write(L_BACK_3_angle);    
    
    prevmicros = micros();
    while(micros() - prevmicros <= turnBwdSpeed) {
    }
  }
  
  else if(L_BACK_1_angle < L_BACK_1_PREVangle && L_BACK_1_angle < L_BACK_1_MAXangle){
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = L_BACK_1_MAXangle + 1;
    turnRightVAR = turnRightVAR + 1;
  }
  
  else if(L_BACK_1_angle > L_BACK_1_PREVangle && L_BACK_1_angle <= L_BACK_1_MINangle){
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = L_BACK_1_angle + 1;
    
    L_BACK_1.write(L_BACK_1_angle);
    
    if(L_BACK_1_angle >= L_BACK_1_UPangle) {
      L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_UPangle, L_BACK_1_MINangle, L_BACK_2_UPangle, L_BACK_2_MINangle);
      L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_UPangle, L_BACK_1_MINangle, L_BACK_3_UPangle, L_BACK_3_MINangle);
      L_BACK_2.write(L_BACK_2_angle);
      L_BACK_3.write(L_BACK_3_angle);
    }
    else if(L_BACK_1_angle < L_BACK_1_UPangle) {
      L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_MAXangle, L_BACK_1_UPangle, L_BACK_2_MAXangle, L_BACK_2_UPangle);
      L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_MAXangle, L_BACK_1_UPangle, L_BACK_3_MAXangle, L_BACK_3_UPangle);
      L_BACK_2.write(L_BACK_2_angle);
      L_BACK_3.write(L_BACK_3_angle);
    }
    
    prevmicros = micros();
    while(micros() - prevmicros <= turnFwdSpeed) {
    }
  }
  
  else if(L_BACK_1_angle > L_BACK_1_PREVangle && L_BACK_1_angle > L_BACK_1_MINangle){
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = L_BACK_1_MINangle - 1;
  }
}

void LeftFrontTRNR() {
  
  /*
  L_BACK_1_angle,    90  ^ 134 < 130 < 105 < 90  // up. 68 
  L_BACK_2_angle,    122 ^ 117 < ??  < ??  < 122 // up.161
  L_BACK_3_angle,    98  ^ 87  < ??  < ??  < 98  // up.100
  
  L_FRONT_1_angle,   90  < 75  ^ 130 < ??  < 90  // up. 78
  L_FRONT_2_angle,   58  < 58  ^ 60  < ??  < 58  // up.19
  L_FRONT_3_angle,   83  < 83  ^ 87  < ??  < 83  // up.80
  
  R_FRONT_1_angle,   90  < 75  < 61  ^ 105 < 90  // up. 112
  R_FRONT_2_angle,   122 < 122 < 120 ^ 121 < 122 // up.161
  R_FRONT_3_angle,   98  < 98  < 93  ^ 96  < 98  // up.100
  
  R_BACK_1_angle,    90  < 75  < ??  < 46  ^ 90  // up. 97
  R_BACK_2_angle,    58  < 58  < ??  < 63  ^ 58  // up.19
  R_BACK_3_angle,    83  < 83  < ??  < 94  ^ 83  // up.80
  */
  
  int L_FRONT_1_UPangle = 75 + ((130 - 75) / 2) ;
  
  if(L_BACK_1_angle > L_BACK_1_PREVangle) {
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(L_BACK_1_angle, 90, 134, 90, 75);
    L_FRONT_2_angle = map(L_BACK_1_angle, 90, 134, 58, 58);
    L_FRONT_3_angle = map(L_BACK_1_angle, 90, 134, 83, 83);
    L_FRONT_1.write(L_FRONT_1_angle);
    L_FRONT_2.write(L_FRONT_2_angle);
    L_FRONT_3.write(L_FRONT_3_angle);
  }
  
  else if(L_BACK_1_angle < L_BACK_1_PREVangle && L_BACK_1_angle >= 130) {
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(L_BACK_1_angle, 134, 130, 75, 130);
    L_FRONT_1.write(L_FRONT_1_angle);
    if(L_FRONT_1_angle <= L_FRONT_1_UPangle) {
      L_FRONT_2_angle = map(L_FRONT_1_angle, 75, L_FRONT_1_UPangle, 58, 19);
      L_FRONT_3_angle = map(L_FRONT_1_angle, 75, L_FRONT_1_UPangle, 83, 80);
      L_FRONT_2.write(L_FRONT_2_angle);
      L_FRONT_3.write(L_FRONT_3_angle);
    }
    else if(L_FRONT_1_angle > L_FRONT_1_UPangle) {
      L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_UPangle, 130, 19, 60);
      L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_UPangle, 130, 80, 87);
      L_FRONT_2.write(L_FRONT_2_angle);
      L_FRONT_3.write(L_FRONT_3_angle);
    }
  }
  
  else if(L_BACK_1_angle < L_BACK_1_PREVangle && L_BACK_1_angle < 130) {
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(L_BACK_1_angle, 130, 90, 130, 90);
    L_FRONT_2_angle = map(L_BACK_1_angle, 130, 90, 60, 58);
    L_FRONT_3_angle = map(L_BACK_1_angle, 130, 90, 87, 83);
    L_FRONT_1.write(L_FRONT_1_angle);
    L_FRONT_2.write(L_FRONT_2_angle);
    L_FRONT_3.write(L_FRONT_3_angle);
  }  
}

void RightFrontTRNR() {
  
  /*
  L_BACK_1_angle,    90  ^ 134 < 130 < 105 < 90  // up. 68 
  L_BACK_2_angle,    122 ^ 117 < ??  < ??  < 122 // up.161
  L_BACK_3_angle,    98  ^ 87  < ??  < ??  < 98  // up.100
  
  L_FRONT_1_angle,   90  < 75  ^ 130 < ??  < 90  // up. 78
  L_FRONT_2_angle,   58  < 58  ^ 60  < ??  < 58  // up.19
  L_FRONT_3_angle,   83  < 83  ^ 87  < ??  < 83  // up.80
  
  R_FRONT_1_angle,   90  < 75  < 61  ^ 105 < 90  // up. 112
  R_FRONT_2_angle,   122 < 122 < 120 ^ 121 < 122 // up.161
  R_FRONT_3_angle,   98  < 98  < 93  ^ 96  < 98  // up.100
  
  R_BACK_1_angle,    90  < 75  < ??  < 46  ^ 90  // up. 97
  R_BACK_2_angle,    58  < 58  < ??  < 63  ^ 58  // up.19
  R_BACK_3_angle,    83  < 83  < ??  < 94  ^ 83  // up.80
  */
  
  int R_FRONT_1_UPangle = 61 + ((105 - 61) / 2);
  
  if(L_BACK_1_angle > L_BACK_1_PREVangle) {
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(L_BACK_1_angle, 90, 134, 90, 75);
    R_FRONT_2_angle = map(L_BACK_1_angle, 90, 134, 122, 122);
    R_FRONT_3_angle = map(L_BACK_1_angle, 90, 134, 98, 98);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }
  else if(L_BACK_1_angle < L_BACK_1_PREVangle && L_BACK_1_angle > 130) {
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(L_BACK_1_angle, 134, 130, 75, 61);
    R_FRONT_2_angle = map(L_BACK_1_angle, 134, 130, 122, 120);
    R_FRONT_3_angle = map(L_BACK_1_angle, 134, 130, 98, 93);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }
  else if(L_BACK_1_angle < L_BACK_1_PREVangle && L_BACK_1_angle <= 130 && L_BACK_1_angle >= 105) {
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(L_BACK_1_angle, 130, 105, 61, 105);
    R_FRONT_1.write(R_FRONT_1_angle);
    if(R_FRONT_1_angle <= R_FRONT_1_UPangle) {
      R_FRONT_2_angle = map(R_FRONT_1_angle, 61, R_FRONT_1_UPangle, 120, 161);
      R_FRONT_3_angle = map(R_FRONT_1_angle, 61, R_FRONT_1_UPangle, 93, 100);
      R_FRONT_2.write(R_FRONT_2_angle);
      R_FRONT_3.write(R_FRONT_3_angle);
    }
    else if(R_FRONT_1_angle > R_FRONT_1_UPangle) {
      R_FRONT_2_angle = map(R_FRONT_1_angle, R_FRONT_1_UPangle, 105, 161, 121);
      R_FRONT_3_angle = map(R_FRONT_1_angle, R_FRONT_1_UPangle, 105, 100, 96);
      R_FRONT_2.write(R_FRONT_2_angle);
      R_FRONT_3.write(R_FRONT_3_angle);
    }
  }
  else if(L_BACK_1_angle < L_BACK_1_PREVangle && L_BACK_1_angle < 105) {
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(L_BACK_1_angle, 105, 90, 105, 90);
    R_FRONT_2_angle = map(L_BACK_1_angle, 105, 90, 121, 122);
    R_FRONT_3_angle = map(L_BACK_1_angle, 105, 90, 96, 98);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }
}

void RightBackTRNR() {
  
  /*
  L_BACK_1_angle,    90  ^ 134 < 130 < 105 < 90  // up. 68 
  L_BACK_2_angle,    122 ^ 117 < ??  < ??  < 122 // up.161
  L_BACK_3_angle,    98  ^ 87  < ??  < ??  < 98  // up.100
  
  L_FRONT_1_angle,   90  < 75  ^ 130 < ??  < 90  // up. 78
  L_FRONT_2_angle,   58  < 58  ^ 60  < ??  < 58  // up.19
  L_FRONT_3_angle,   83  < 83  ^ 87  < ??  < 83  // up.80
  
  R_FRONT_1_angle,   90  < 75  < 61  ^ 105 < 90  // up. 112
  R_FRONT_2_angle,   122 < 122 < 120 ^ 121 < 122 // up.161
  R_FRONT_3_angle,   98  < 98  < 93  ^ 96  < 98  // up.100
  
  R_BACK_1_angle,    90  < 75  < ??  < 46  ^ 90  // up. 97
  R_BACK_2_angle,    58  < 58  < ??  < 63  ^ 58  // up.19
  R_BACK_3_angle,    83  < 83  < ??  < 94  ^ 83  // up.80
  */

  int R_BACK_1_UPangle = 90 - ((90 - 46) / 2);

  if(L_BACK_1_angle > L_BACK_1_PREVangle) {
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = map(L_BACK_1_angle, 90, 134, 90, 75);
    R_BACK_2_angle = map(L_BACK_1_angle, 90, 134, 58, 58);
    R_BACK_3_angle = map(L_BACK_1_angle, 90, 134, 83, 83);
    R_BACK_1.write(R_BACK_1_angle);
    R_BACK_2.write(R_BACK_2_angle);
    R_BACK_3.write(R_BACK_3_angle);
  }
  else if(L_BACK_1_angle < L_BACK_1_PREVangle && L_BACK_1_angle > 105) {
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = map(L_BACK_1_angle, 134, 105, 75, 46);
    R_BACK_2_angle = map(L_BACK_1_angle, 134, 105, 58, 63);
    R_BACK_3_angle = map(L_BACK_1_angle, 134, 105, 83, 94);
    R_BACK_1.write(R_BACK_1_angle);
    R_BACK_2.write(R_BACK_2_angle);
    R_BACK_3.write(R_BACK_3_angle);
  }
  else if(L_BACK_1_angle < L_BACK_1_PREVangle && L_BACK_1_angle <= 105) {
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = map(L_BACK_1_angle, 105, 90, 46, 90);
    R_BACK_1.write(R_BACK_1_angle);
    if(R_BACK_1_angle <= R_BACK_1_UPangle) {
      R_BACK_2_angle = map(R_BACK_1_angle, 46, R_BACK_1_UPangle, 63, 19);
      R_BACK_3_angle = map(R_BACK_1_angle, 46, R_BACK_1_UPangle, 94, 80);
      R_BACK_2.write(R_BACK_2_angle);
      R_BACK_3.write(R_BACK_3_angle);
    }
    else if(R_BACK_1_angle > R_BACK_1_UPangle) {
      R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_UPangle, 90, 19, 58);
      R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_UPangle, 90, 80, 83);
      R_BACK_2.write(R_BACK_2_angle);
      R_BACK_3.write(R_BACK_3_angle);
    }
  }
}

/* ------------------------------- END OF TURN RIGHT ----------------------------------------------*/

/* ------------------------------- WALK FORWARD ----------------------------------------------*/

void LeftFrontFWD() {
  
  // L_BACK_1_angle,  min.135, std.90, up.102, switch.128/121, max.83
  // L_FRONT_1_angle, min.45,  std.90, up.79,  switch.53/61,   max.98
  // L_FRONT_2_angle, min.49,  std.59, up.19,  switch.52/55,   max.69
  // L_FRONT_3_angle, min.52,  std.83, up.80,  switch.60/68,   max.106
  
  int L_BACK_1_MAXangle = 83;
  int L_BACK_1_MINangle = 135;
  int L_FRONT_1_MAXangle = 98;
  int L_FRONT_1_MINangle = 45;
  int L_FRONT_1_UPangle = L_FRONT_1_MINangle + ((L_FRONT_1_MAXangle - L_FRONT_1_MINangle) / 2);
  int L_FRONT_2_MAXangle = 69;
  int L_FRONT_2_MINangle = 49;
  int L_FRONT_2_UPangle = 19;
  int L_FRONT_3_MAXangle = 106;
  int L_FRONT_3_MINangle = 52;
  int L_FRONT_3_UPangle = 80;
  
  int L_BACK_1_SWITCHmin = L_BACK_1_MINangle - ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 7);
  int L_BACK_1_SWITCHmax = L_BACK_1_SWITCHmin - ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 7);
  
  int L_FRONT_1_SWITCHmin = L_FRONT_1_MINangle + ((L_FRONT_1_MAXangle - L_FRONT_1_MINangle) / 7);
  int L_FRONT_1_SWITCHmax = L_FRONT_1_SWITCHmin + ((L_FRONT_1_MAXangle - L_FRONT_1_MINangle) / 7);
  
  int L_FRONT_2_SWITCHmin = L_FRONT_2_MINangle + ((L_FRONT_2_MAXangle - L_FRONT_2_MINangle) / 7);
  int L_FRONT_2_SWITCHmax = L_FRONT_2_SWITCHmin + ((L_FRONT_2_MAXangle - L_FRONT_2_MINangle) / 7);
  
  int L_FRONT_3_SWITCHmin = L_FRONT_3_MINangle + ((L_FRONT_3_MAXangle - L_FRONT_3_MINangle) / 7);
  int L_FRONT_3_SWITCHmax = L_FRONT_3_SWITCHmin + ((L_FRONT_3_MAXangle - L_FRONT_3_MINangle) / 7);
  
  if(L_BACK_1_angle > L_BACK_1_SWITCHmin && L_BACK_1_angle < L_BACK_1_PREVangle) {
    L_FRONT_stepUP = 0;
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_SWITCHmin, L_FRONT_1_SWITCHmin, L_FRONT_1_MINangle);
    L_FRONT_2_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_SWITCHmin, L_FRONT_2_SWITCHmin, L_FRONT_2_MINangle);
    L_FRONT_3_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_SWITCHmin, L_FRONT_3_SWITCHmin, L_FRONT_3_MINangle);
    L_FRONT_1.write(L_FRONT_1_angle);
    L_FRONT_2.write(L_FRONT_2_angle);
    L_FRONT_3.write(L_FRONT_3_angle);
  }
  else if(L_BACK_1_angle <= L_BACK_1_SWITCHmin && L_BACK_1_angle >= L_BACK_1_SWITCHmax && L_BACK_1_angle < L_BACK_1_PREVangle) {
    L_FRONT_stepUP = 1;
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(L_BACK_1_angle, L_BACK_1_SWITCHmin, L_BACK_1_SWITCHmax, L_FRONT_1_MINangle, L_FRONT_1_MAXangle);
    L_FRONT_1.write(L_FRONT_1_angle);
    if(L_FRONT_1_angle >= L_FRONT_1_UPangle) {
      L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_UPangle, L_FRONT_1_MAXangle, L_FRONT_2_UPangle, L_FRONT_2_MAXangle);
      L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_UPangle, L_FRONT_1_MAXangle, L_FRONT_3_UPangle, L_FRONT_3_MAXangle);
      L_FRONT_2.write(L_FRONT_2_angle);
      L_FRONT_3.write(L_FRONT_3_angle);
    }
    else if(L_FRONT_1_angle < L_FRONT_1_UPangle) {
      L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_MINangle, L_FRONT_1_UPangle, L_FRONT_2_MINangle, L_FRONT_2_UPangle);
      L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_MINangle, L_FRONT_1_UPangle, L_FRONT_3_MINangle, L_FRONT_3_UPangle);
      L_FRONT_2.write(L_FRONT_2_angle);
      L_FRONT_3.write(L_FRONT_3_angle);
    }
  }
  else if(L_BACK_1_angle < L_BACK_1_SWITCHmax && L_BACK_1_angle < L_BACK_1_PREVangle) {
    L_FRONT_stepUP = 0;
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(L_BACK_1_angle, L_BACK_1_SWITCHmax, L_BACK_1_MAXangle, L_FRONT_1_MAXangle, L_FRONT_1_SWITCHmax);
    L_FRONT_2_angle = map(L_BACK_1_angle, L_BACK_1_SWITCHmax, L_BACK_1_MAXangle, L_FRONT_2_MAXangle, L_FRONT_2_SWITCHmax);
    L_FRONT_3_angle = map(L_BACK_1_angle, L_BACK_1_SWITCHmax, L_BACK_1_MAXangle, L_FRONT_3_MAXangle, L_FRONT_3_SWITCHmax);
    L_FRONT_1.write(L_FRONT_1_angle);
    L_FRONT_2.write(L_FRONT_2_angle);
    L_FRONT_3.write(L_FRONT_3_angle);
  }
  else if(L_BACK_1_angle > L_BACK_1_PREVangle) {
    L_FRONT_stepUP = 0;
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = map(L_BACK_1_angle, L_BACK_1_MAXangle, L_BACK_1_MINangle, L_FRONT_1_SWITCHmax, L_FRONT_1_SWITCHmin);
    L_FRONT_2_angle = map(L_BACK_1_angle, L_BACK_1_MAXangle, L_BACK_1_MINangle, L_FRONT_2_SWITCHmax, L_FRONT_2_SWITCHmin);
    L_FRONT_3_angle = map(L_BACK_1_angle, L_BACK_1_MAXangle, L_BACK_1_MINangle, L_FRONT_3_SWITCHmax, L_FRONT_3_SWITCHmin);
    L_FRONT_1.write(L_FRONT_1_angle);
    L_FRONT_2.write(L_FRONT_2_angle);
    L_FRONT_3.write(L_FRONT_3_angle);
  }
}

void LeftBackFWD() {
  
  // R_FRONT_1_angle, min.135, std.90,  up.102, switch.97/90,   max.83
  // L_BACK_1_angle,  min.135, std.90,  up.102, switch.97/90,   max.83
  // L_BACK_2_angle,  min.132, std.122, up.161, switch.114/110, max.111
  // L_BACK_3_angle,  min.129, std.98,  up.100, switch.91/83,   max.75

  int R_FRONT_1_MAXangle = 83;
  int R_FRONT_1_MINangle = 135;
  int L_BACK_1_MAXangle = 83;
  int L_BACK_1_MINangle = 135;
  int L_BACK_1_UPangle = L_BACK_1_MAXangle + ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 2);
  int L_BACK_2_MAXangle = 111;
  int L_BACK_2_MINangle = 132;
  int L_BACK_2_UPangle = 161;
  int L_BACK_3_MAXangle = 75;
  int L_BACK_3_MINangle = 129;
  int L_BACK_3_UPangle = 100;
  
  int R_FRONT_1_SWITCHmax = R_FRONT_1_MAXangle + ((R_FRONT_1_MINangle - R_FRONT_1_MAXangle) / 7);
  int R_FRONT_1_SWITCHmin = R_FRONT_1_SWITCHmax + ((R_FRONT_1_MINangle - R_FRONT_1_MAXangle) / 7);
  
  int L_BACK_1_SWITCHmax = L_BACK_1_MAXangle + ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 7);
  int L_BACK_1_SWITCHmin = L_BACK_1_SWITCHmax + ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 7);
  
  int L_BACK_2_SWITCHmax = L_BACK_2_MAXangle + ((L_BACK_2_MINangle - L_BACK_2_MAXangle) / 7);
  int L_BACK_2_SWITCHmin = L_BACK_2_SWITCHmax + ((L_BACK_2_MINangle - L_BACK_2_MAXangle) / 7);
  
  int L_BACK_3_SWITCHmax = L_BACK_3_MAXangle + ((L_BACK_3_MINangle - L_BACK_3_MAXangle) / 7);
  int L_BACK_3_SWITCHmin = L_BACK_3_SWITCHmax + ((L_BACK_3_MINangle - L_BACK_3_MAXangle) / 7);

  if(R_FRONT_1_angle < R_FRONT_1_SWITCHmax && R_FRONT_1_angle > R_FRONT_1_PREVangle) {
    L_BACK_stepUP = 0;
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(R_FRONT_1_angle, R_FRONT_1_MAXangle, R_FRONT_1_SWITCHmax, L_BACK_1_SWITCHmax, L_BACK_1_MAXangle);
    L_BACK_2_angle = map(R_FRONT_1_angle, R_FRONT_1_MAXangle, R_FRONT_1_SWITCHmax, L_BACK_2_SWITCHmax, L_BACK_2_MAXangle);
    L_BACK_3_angle = map(R_FRONT_1_angle, R_FRONT_1_MAXangle, R_FRONT_1_SWITCHmax, L_BACK_3_SWITCHmax, L_BACK_3_MAXangle);
    L_BACK_1.write(L_BACK_1_angle);
    L_BACK_2.write(L_BACK_2_angle);
    L_BACK_3.write(L_BACK_3_angle);
  }
  else if(R_FRONT_1_angle >= R_FRONT_1_SWITCHmax && R_FRONT_1_angle <= R_FRONT_1_SWITCHmin && R_FRONT_1_angle > R_FRONT_1_PREVangle) {
    L_BACK_stepUP = 1;
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(R_FRONT_1_angle, R_FRONT_1_SWITCHmax, R_FRONT_1_SWITCHmin, L_BACK_1_MAXangle, L_BACK_1_MINangle);
    L_BACK_1.write(L_BACK_1_angle);
    if(L_BACK_1_angle >= L_BACK_1_UPangle) {
      L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_UPangle, L_BACK_2_MINangle, L_BACK_2_UPangle);
      L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_UPangle, L_BACK_3_MINangle, L_BACK_3_UPangle);
      L_BACK_2.write(L_BACK_2_angle);
      L_BACK_3.write(L_BACK_3_angle);
    }
    else if(L_BACK_1_angle < L_BACK_1_UPangle) {
      L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_UPangle, L_BACK_1_MAXangle, L_BACK_2_UPangle, L_BACK_2_MAXangle);
      L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_UPangle, L_BACK_1_MAXangle, L_BACK_3_UPangle, L_BACK_3_MAXangle);
      L_BACK_2.write(L_BACK_2_angle);
      L_BACK_3.write(L_BACK_3_angle);
    }
  }
  else if(R_FRONT_1_angle > R_FRONT_1_SWITCHmin && R_FRONT_1_angle > R_FRONT_1_PREVangle) {
    L_BACK_stepUP = 0;
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(R_FRONT_1_angle, R_FRONT_1_SWITCHmin, R_FRONT_1_MINangle, L_BACK_1_MINangle, L_BACK_1_SWITCHmin);
    L_BACK_2_angle = map(R_FRONT_1_angle, R_FRONT_1_SWITCHmin, R_FRONT_1_MINangle, L_BACK_2_MINangle, L_BACK_2_SWITCHmin);
    L_BACK_3_angle = map(R_FRONT_1_angle, R_FRONT_1_SWITCHmin, R_FRONT_1_MINangle, L_BACK_3_MINangle, L_BACK_3_SWITCHmin);
    L_BACK_1.write(L_BACK_1_angle);
    L_BACK_2.write(L_BACK_2_angle);
    L_BACK_3.write(L_BACK_3_angle);
  }
  else if(R_FRONT_1_angle < R_FRONT_1_PREVangle) {
    L_BACK_stepUP = 0;
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(R_FRONT_1_angle, R_FRONT_1_MINangle, R_FRONT_1_MAXangle, L_BACK_1_SWITCHmin, L_BACK_1_SWITCHmax);
    L_BACK_2_angle = map(R_FRONT_1_angle, R_FRONT_1_MINangle, R_FRONT_1_MAXangle, L_BACK_2_SWITCHmin, L_BACK_2_SWITCHmax);
    L_BACK_3_angle = map(R_FRONT_1_angle, R_FRONT_1_MINangle, R_FRONT_1_MAXangle, L_BACK_3_SWITCHmin, L_BACK_3_SWITCHmax);
    L_BACK_1.write(L_BACK_1_angle);
    L_BACK_2.write(L_BACK_2_angle);
    L_BACK_3.write(L_BACK_3_angle);
  }
}

void RightFrontFWD() {
  
  // R_BACK_1_angle,  min.45,  std.90,  up.79,  switch.53/61,    max.98
  // R_FRONT_1_angle, min.135, std.90,  up.102, switch.128/121,  max.83
  // R_FRONT_2_angle, min.132, std.122, up.161, switch.130/128,  max.111
  // R_FRONT_3_angle, min.129, std.98,  up.100, switch.119/109,  max.75
  
  int R_BACK_1_MAXangle = 98;
  int R_BACK_1_MINangle = 45;
  int R_FRONT_1_MAXangle = 83;
  int R_FRONT_1_MINangle = 135;
  int R_FRONT_1_UPangle = R_FRONT_1_MAXangle + ((R_FRONT_1_MINangle - R_FRONT_1_MAXangle) / 2) ;
  int R_FRONT_2_MAXangle = 111;
  int R_FRONT_2_MINangle = 132;
  int R_FRONT_2_UPangle = 161;
  int R_FRONT_3_MAXangle = 75;
  int R_FRONT_3_MINangle = 129;
  int R_FRONT_3_UPangle = 100;
  
  int R_BACK_1_SWITCHmin = R_BACK_1_MINangle + ((R_BACK_1_MAXangle - R_BACK_1_MINangle) / 7);
  int R_BACK_1_SWITCHmax = R_BACK_1_SWITCHmin + ((R_BACK_1_MAXangle - R_BACK_1_MINangle) / 7);
  
  int R_FRONT_1_SWITCHmin = R_FRONT_1_MINangle - ((R_FRONT_1_MINangle - R_FRONT_1_MAXangle) / 7);
  int R_FRONT_1_SWITCHmax = R_FRONT_1_SWITCHmin - ((R_FRONT_1_MINangle - R_FRONT_1_MAXangle) / 7);
  
  int R_FRONT_2_SWITCHmin = R_FRONT_2_MINangle - ((R_FRONT_2_MINangle - R_FRONT_2_MAXangle) / 7);
  int R_FRONT_2_SWITCHmax = R_FRONT_2_SWITCHmin - ((R_FRONT_2_MINangle - R_FRONT_2_MAXangle) / 7);
  
  int R_FRONT_3_SWITCHmin = R_FRONT_3_MINangle - ((R_FRONT_3_MINangle - R_FRONT_3_MAXangle) / 7);
  int R_FRONT_3_SWITCHmax = R_FRONT_3_SWITCHmin - ((R_FRONT_3_MINangle - R_FRONT_3_MAXangle) / 7);
  
  if(R_BACK_1_angle < R_BACK_1_SWITCHmin && R_BACK_1_angle > R_BACK_1_PREVangle) {
    R_FRONT_stepUP = 0;
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(R_BACK_1_angle, R_BACK_1_MINangle, R_BACK_1_SWITCHmin, R_FRONT_1_SWITCHmin, R_FRONT_1_MINangle);
    R_FRONT_2_angle = map(R_BACK_1_angle, R_BACK_1_MINangle, R_BACK_1_SWITCHmin, R_FRONT_2_SWITCHmin, R_FRONT_2_MINangle);
    R_FRONT_3_angle = map(R_BACK_1_angle, R_BACK_1_MINangle, R_BACK_1_SWITCHmin, R_FRONT_3_SWITCHmin, R_FRONT_3_MINangle);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }
  else if(R_BACK_1_angle >= R_BACK_1_SWITCHmin && R_BACK_1_angle <= R_BACK_1_SWITCHmax && R_BACK_1_angle > R_BACK_1_PREVangle) {
    R_FRONT_stepUP = 1;
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(R_BACK_1_angle, R_BACK_1_SWITCHmin, R_BACK_1_SWITCHmax, R_FRONT_1_MINangle, R_FRONT_1_MAXangle);
    R_FRONT_1.write(R_FRONT_1_angle);
    if(R_FRONT_1_angle >= R_FRONT_1_UPangle) {
      R_FRONT_2_angle = map(R_FRONT_1_angle, R_FRONT_1_MINangle, R_FRONT_1_UPangle, R_FRONT_2_MINangle, R_FRONT_2_UPangle);
      R_FRONT_3_angle = map(R_FRONT_1_angle, R_FRONT_1_MINangle, R_FRONT_1_UPangle, R_FRONT_3_MINangle, R_FRONT_3_UPangle);
      R_FRONT_2.write(R_FRONT_2_angle);
      R_FRONT_3.write(R_FRONT_3_angle);
    }
    else if(R_FRONT_1_angle < R_FRONT_1_UPangle) {
      R_FRONT_2_angle = map(R_FRONT_1_angle, R_FRONT_1_UPangle, R_FRONT_1_MAXangle, R_FRONT_2_UPangle, R_FRONT_2_MAXangle);
      R_FRONT_3_angle = map(R_FRONT_1_angle, R_FRONT_1_UPangle, R_FRONT_1_MAXangle, R_FRONT_3_UPangle, R_FRONT_3_MAXangle);
      R_FRONT_2.write(R_FRONT_2_angle);
      R_FRONT_3.write(R_FRONT_3_angle);
    }
  }
  else if(R_BACK_1_angle > R_BACK_1_SWITCHmax && R_BACK_1_angle > R_BACK_1_PREVangle) {
    R_FRONT_stepUP =0;
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(R_BACK_1_angle, R_BACK_1_SWITCHmax, R_BACK_1_MAXangle, R_FRONT_1_MAXangle, R_FRONT_1_SWITCHmax);
    R_FRONT_2_angle = map(R_BACK_1_angle, R_BACK_1_SWITCHmax, R_BACK_1_MAXangle, R_FRONT_2_MAXangle, R_FRONT_2_SWITCHmax);
    R_FRONT_3_angle = map(R_BACK_1_angle, R_BACK_1_SWITCHmax, R_BACK_1_MAXangle, R_FRONT_3_MAXangle, R_FRONT_3_SWITCHmax);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }
  else if(R_BACK_1_angle < R_BACK_1_PREVangle) {
    R_FRONT_stepUP = 0;
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(R_BACK_1_angle, R_BACK_1_MAXangle, R_BACK_1_MINangle, R_FRONT_1_SWITCHmax, R_FRONT_1_SWITCHmin);
    R_FRONT_2_angle = map(R_BACK_1_angle, R_BACK_1_MAXangle, R_BACK_1_MINangle, R_FRONT_2_SWITCHmax, R_FRONT_2_SWITCHmin);
    R_FRONT_3_angle = map(R_BACK_1_angle, R_BACK_1_MAXangle, R_BACK_1_MINangle, R_FRONT_3_SWITCHmax, R_FRONT_3_SWITCHmin);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }
}

void RightBackFWD() {
  
  // R_BACK_1_angle, min.45, std.90, up.79, switch.63/80, max.98
  // R_BACK_2_angle, min.49, std.59, up.19, switch.56/62, max.69
  // R_BACK_3_angle, min.52, std.83, up.80, switch.70/88, max.106
  
  int R_BACK_1_MAXangle = 98;
  int R_BACK_1_MINangle = 45;
  int R_BACK_1_UPangle = R_BACK_1_MINangle + ((R_BACK_1_MAXangle - R_BACK_1_MINangle) / 2);
  int R_BACK_2_MAXangle = 69;
  int R_BACK_2_MINangle = 49;
  int R_BACK_2_UPangle = 19;
  int R_BACK_3_MAXangle = 106;
  int R_BACK_3_MINangle = 52;
  int R_BACK_3_UPangle = 80;
  
  if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle <= R_BACK_1_MAXangle) {
    R_BACK_stepUP = 0;
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = R_BACK_1_angle + 1;
    
    R_BACK_1.write(R_BACK_1_angle);
    R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_MINangle, R_BACK_1_MAXangle, R_BACK_2_MINangle, R_BACK_2_MAXangle);
    R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_MINangle, R_BACK_1_MAXangle, R_BACK_3_MINangle, R_BACK_3_MAXangle);
    R_BACK_2.write(R_BACK_2_angle);
    R_BACK_3.write(R_BACK_3_angle);    
    
    prevmicros = micros();
    while(micros() - prevmicros <= stepBwdSpeed) {
    }
  }
  
  else if(R_BACK_1_angle > R_BACK_1_PREVangle && R_BACK_1_angle > R_BACK_1_MAXangle){
    R_BACK_stepUP = 1;
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = R_BACK_1_MAXangle - 1;
  }
  
  else if(R_BACK_1_angle < R_BACK_1_PREVangle && R_BACK_1_angle >= R_BACK_1_MINangle){
    R_BACK_stepUP = 1;
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = R_BACK_1_angle - 1;
    
    R_BACK_1.write(R_BACK_1_angle);
    
    if(R_BACK_1_angle <= R_BACK_1_UPangle) {
      R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_UPangle, R_BACK_1_MINangle, R_BACK_2_UPangle, R_BACK_2_MINangle);
      R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_UPangle, R_BACK_1_MINangle, R_BACK_3_UPangle, R_BACK_3_MINangle);
      R_BACK_2.write(R_BACK_2_angle);
      R_BACK_3.write(R_BACK_3_angle);
    }
    else if(R_BACK_1_angle > R_BACK_1_UPangle) {
      R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_MAXangle, R_BACK_1_UPangle, R_BACK_2_MAXangle, R_BACK_2_UPangle);
      R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_MAXangle, R_BACK_1_UPangle, R_BACK_3_MAXangle, R_BACK_3_UPangle);
      R_BACK_2.write(R_BACK_2_angle);
      R_BACK_3.write(R_BACK_3_angle);
    }
    
    prevmicros = micros();
    while(micros() - prevmicros <= stepFwdSpeed) {
    }
  }
  
  else if(R_BACK_1_angle < R_BACK_1_PREVangle && R_BACK_1_angle < R_BACK_1_MINangle){
    R_BACK_stepUP = 0;
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = R_BACK_1_MINangle + 1;
  }
}

/* ------------------------------- END OF WALK FORWARD ----------------------------------------------*/

/* ------------------------------- WALK BACKWARD ----------------------------------------------*/

void RightBackBWD() {
  
  // R_FRONT_1_angle, min.135, std.90, up.102, switch.128/121, max.83
  // R_BACK_1_angle,  min.45,  std.90, up.79,  switch.53/61,   max.98
  // R_BACK_2_angle,  min.49,  std.59, up.19,  switch.52/55,   max.69
  // R_BACK_3_angle,  min.52,  std.83, up.80,  switch.60/68,   max.106
  
  int R_FRONT_1_MAXangle = 83;
  int R_FRONT_1_MINangle = 135;
  int R_BACK_1_MAXangle = 98;
  int R_BACK_1_MINangle = 45;
  int R_BACK_1_UPangle = R_BACK_1_MINangle + ((R_BACK_1_MAXangle - R_BACK_1_MINangle) / 2);
  int R_BACK_2_MAXangle = 69;
  int R_BACK_2_MINangle = 49;
  int R_BACK_2_UPangle = 19;
  int R_BACK_3_MAXangle = 106;
  int R_BACK_3_MINangle = 52;
  int R_BACK_3_UPangle = 80;
  
  int R_FRONT_1_SWITCHmin = R_FRONT_1_MINangle - ((R_FRONT_1_MINangle - R_FRONT_1_MAXangle) / 7);
  int R_FRONT_1_SWITCHmax = R_FRONT_1_SWITCHmin - ((R_FRONT_1_MINangle - R_FRONT_1_MAXangle) / 7);
  
  int R_BACK_1_SWITCHmin = R_BACK_1_MINangle + ((R_BACK_1_MAXangle - R_BACK_1_MINangle) / 7);
  int R_BACK_1_SWITCHmax = R_BACK_1_SWITCHmin + ((R_BACK_1_MAXangle - R_BACK_1_MINangle) / 7);
  
  int R_BACK_2_SWITCHmin = R_BACK_2_MINangle + ((R_BACK_2_MAXangle - R_BACK_2_MINangle) / 7);
  int R_BACK_2_SWITCHmax = R_BACK_2_SWITCHmin + ((R_BACK_2_MAXangle - R_BACK_2_MINangle) / 7);
  
  int R_BACK_3_SWITCHmin = R_BACK_3_MINangle + ((R_BACK_3_MAXangle - R_BACK_3_MINangle) / 7);
  int R_BACK_3_SWITCHmax = R_BACK_3_SWITCHmin + ((R_BACK_3_MAXangle - R_BACK_3_MINangle) / 7);
  
  if(R_FRONT_1_angle > R_FRONT_1_SWITCHmin && R_FRONT_1_angle < R_FRONT_1_PREVangle) {
    R_BACK_stepUP = 0;
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = map(R_FRONT_1_angle, R_FRONT_1_MINangle, R_FRONT_1_SWITCHmin, R_BACK_1_SWITCHmin, R_BACK_1_MINangle);
    R_BACK_2_angle = map(R_FRONT_1_angle, R_FRONT_1_MINangle, R_FRONT_1_SWITCHmin, R_BACK_2_SWITCHmin, R_BACK_2_MINangle);
    R_BACK_3_angle = map(R_FRONT_1_angle, R_FRONT_1_MINangle, R_FRONT_1_SWITCHmin, R_BACK_3_SWITCHmin, R_BACK_3_MINangle);
    R_BACK_1.write(R_BACK_1_angle);
    R_BACK_2.write(R_BACK_2_angle);
    R_BACK_3.write(R_BACK_3_angle);
  }
  else if(R_FRONT_1_angle <= R_FRONT_1_SWITCHmin && R_FRONT_1_angle >= R_FRONT_1_SWITCHmax && R_FRONT_1_angle < R_FRONT_1_PREVangle) {
    R_BACK_stepUP = 1;
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = map(R_FRONT_1_angle, R_FRONT_1_SWITCHmin, R_FRONT_1_SWITCHmax, R_BACK_1_MINangle, R_BACK_1_MAXangle);
    R_BACK_1.write(R_BACK_1_angle);
    if(R_BACK_1_angle >= R_BACK_1_UPangle) {
      R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_UPangle, R_BACK_1_MAXangle, R_BACK_2_UPangle, R_BACK_2_MAXangle);
      R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_UPangle, R_BACK_1_MAXangle, R_BACK_3_UPangle, R_BACK_3_MAXangle);
      R_BACK_2.write(R_BACK_2_angle);
      R_BACK_3.write(R_BACK_3_angle);
    }
    else if(R_BACK_1_angle < R_BACK_1_UPangle) {
      R_BACK_2_angle = map(R_BACK_1_angle, R_BACK_1_MINangle, R_BACK_1_UPangle, R_BACK_2_MINangle, R_BACK_2_UPangle);
      R_BACK_3_angle = map(R_BACK_1_angle, R_BACK_1_MINangle, R_BACK_1_UPangle, R_BACK_3_MINangle, R_BACK_3_UPangle);
      R_BACK_2.write(R_BACK_2_angle);
      R_BACK_3.write(R_BACK_3_angle);
    }
  }
  else if(R_FRONT_1_angle < R_FRONT_1_SWITCHmax && R_FRONT_1_angle < R_FRONT_1_PREVangle) {
    R_BACK_stepUP = 0;
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = map(R_FRONT_1_angle, R_FRONT_1_SWITCHmax, R_FRONT_1_MAXangle, R_BACK_1_MAXangle, R_BACK_1_SWITCHmax);
    R_BACK_2_angle = map(R_FRONT_1_angle, R_FRONT_1_SWITCHmax, R_FRONT_1_MAXangle, R_BACK_2_MAXangle, R_BACK_2_SWITCHmax);
    R_BACK_3_angle = map(R_FRONT_1_angle, R_FRONT_1_SWITCHmax, R_FRONT_1_MAXangle, R_BACK_3_MAXangle, R_BACK_3_SWITCHmax);
    R_BACK_1.write(R_BACK_1_angle);
    R_BACK_2.write(R_BACK_2_angle);
    R_BACK_3.write(R_BACK_3_angle);
  }
  else if(R_FRONT_1_angle > R_FRONT_1_PREVangle) {
    R_BACK_stepUP = 0;
    R_BACK_1_PREVangle = R_BACK_1_angle;
    R_BACK_1_angle = map(R_FRONT_1_angle, R_FRONT_1_MAXangle, R_FRONT_1_MINangle, R_BACK_1_SWITCHmax, R_BACK_1_SWITCHmin);
    R_BACK_2_angle = map(R_FRONT_1_angle, R_FRONT_1_MAXangle, R_FRONT_1_MINangle, R_BACK_2_SWITCHmax, R_BACK_2_SWITCHmin);
    R_BACK_3_angle = map(R_FRONT_1_angle, R_FRONT_1_MAXangle, R_FRONT_1_MINangle, R_BACK_3_SWITCHmax, R_BACK_3_SWITCHmin);
    R_BACK_1.write(R_BACK_1_angle);
    R_BACK_2.write(R_BACK_2_angle);
    R_BACK_3.write(R_BACK_3_angle);
  }
}

void RightFrontBWD() {
  
  // L_BACK_1_angle,  min.135, std.90,  up.102, switch.97/90,   max.83
  // R_FRONT_1_angle, min.135, std.90,  up.102, switch.97/90,   max.83
  // R_FRONT_2_angle, min.132, std.122, up.161, switch.114/110, max.111
  // R_FRONT_3_angle, min.129, std.98,  up.100, switch.91/83,   max.75

  int L_BACK_1_MAXangle = 83;
  int L_BACK_1_MINangle = 135;
  int R_FRONT_1_MAXangle = 83;
  int R_FRONT_1_MINangle = 135;
  int R_FRONT_1_UPangle = R_FRONT_1_MAXangle + ((R_FRONT_1_MINangle - R_FRONT_1_MAXangle) / 2);
  int R_FRONT_2_MAXangle = 111;
  int R_FRONT_2_MINangle = 132;
  int R_FRONT_2_UPangle = 161;
  int R_FRONT_3_MAXangle = 75;
  int R_FRONT_3_MINangle = 129;
  int R_FRONT_3_UPangle = 100;
  
  int L_BACK_1_SWITCHmax = L_BACK_1_MAXangle + ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 7);
  int L_BACK_1_SWITCHmin = L_BACK_1_SWITCHmax + ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 7);
  
  int R_FRONT_1_SWITCHmax = R_FRONT_1_MAXangle + ((R_FRONT_1_MINangle - R_FRONT_1_MAXangle) / 7);
  int R_FRONT_1_SWITCHmin = R_FRONT_1_SWITCHmax + ((R_FRONT_1_MINangle - R_FRONT_1_MAXangle) / 7);
  
  int R_FRONT_2_SWITCHmax = R_FRONT_2_MAXangle + ((R_FRONT_2_MINangle - R_FRONT_2_MAXangle) / 7);
  int R_FRONT_2_SWITCHmin = R_FRONT_2_SWITCHmax + ((R_FRONT_2_MINangle - R_FRONT_2_MAXangle) / 7);
  
  int R_FRONT_3_SWITCHmax = R_FRONT_3_MAXangle + ((R_FRONT_3_MINangle - R_FRONT_3_MAXangle) / 7);
  int R_FRONT_3_SWITCHmin = R_FRONT_3_SWITCHmax + ((R_FRONT_3_MINangle - R_FRONT_3_MAXangle) / 7);

  if(L_BACK_1_angle < L_BACK_1_SWITCHmax && L_BACK_1_angle > L_BACK_1_PREVangle) {
    R_FRONT_stepUP = 0;
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(L_BACK_1_angle, L_BACK_1_MAXangle, L_BACK_1_SWITCHmax, R_FRONT_1_SWITCHmax, R_FRONT_1_MAXangle);
    R_FRONT_2_angle = map(L_BACK_1_angle, L_BACK_1_MAXangle, L_BACK_1_SWITCHmax, R_FRONT_2_SWITCHmax, R_FRONT_2_MAXangle);
    R_FRONT_3_angle = map(L_BACK_1_angle, L_BACK_1_MAXangle, L_BACK_1_SWITCHmax, R_FRONT_3_SWITCHmax, R_FRONT_3_MAXangle);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }
  else if(L_BACK_1_angle >= L_BACK_1_SWITCHmax && L_BACK_1_angle <= L_BACK_1_SWITCHmin && L_BACK_1_angle > L_BACK_1_PREVangle) {
    R_FRONT_stepUP = 1;
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(L_BACK_1_angle, L_BACK_1_SWITCHmax, L_BACK_1_SWITCHmin, R_FRONT_1_MAXangle, R_FRONT_1_MINangle);
    R_FRONT_1.write(R_FRONT_1_angle);
    if(R_FRONT_1_angle >= R_FRONT_1_UPangle) {
      R_FRONT_2_angle = map(R_FRONT_1_angle, R_FRONT_1_MINangle, R_FRONT_1_UPangle, R_FRONT_2_MINangle, R_FRONT_2_UPangle);
      R_FRONT_3_angle = map(R_FRONT_1_angle, R_FRONT_1_MINangle, R_FRONT_1_UPangle, R_FRONT_3_MINangle, R_FRONT_3_UPangle);
      R_FRONT_2.write(R_FRONT_2_angle);
      R_FRONT_3.write(R_FRONT_3_angle);
    }
    else if(R_FRONT_1_angle < R_FRONT_1_UPangle) {
      R_FRONT_2_angle = map(R_FRONT_1_angle, R_FRONT_1_UPangle, R_FRONT_1_MAXangle, R_FRONT_2_UPangle, R_FRONT_2_MAXangle);
      R_FRONT_3_angle = map(R_FRONT_1_angle, R_FRONT_1_UPangle, R_FRONT_1_MAXangle, R_FRONT_3_UPangle, R_FRONT_3_MAXangle);
      R_FRONT_2.write(R_FRONT_2_angle);
      R_FRONT_3.write(R_FRONT_3_angle);
    }
  }
  else if(L_BACK_1_angle > L_BACK_1_SWITCHmin && L_BACK_1_angle > L_BACK_1_PREVangle) {
    R_FRONT_stepUP = 0;
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(L_BACK_1_angle, L_BACK_1_SWITCHmin, L_BACK_1_MINangle, R_FRONT_1_MINangle, R_FRONT_1_SWITCHmin);
    R_FRONT_2_angle = map(L_BACK_1_angle, L_BACK_1_SWITCHmin, L_BACK_1_MINangle, R_FRONT_2_MINangle, R_FRONT_2_SWITCHmin);
    R_FRONT_3_angle = map(L_BACK_1_angle, L_BACK_1_SWITCHmin, L_BACK_1_MINangle, R_FRONT_3_MINangle, R_FRONT_3_SWITCHmin);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }
  else if(L_BACK_1_angle < L_BACK_1_PREVangle) {
    R_FRONT_stepUP = 0;
    R_FRONT_1_PREVangle = R_FRONT_1_angle;
    R_FRONT_1_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_MAXangle, R_FRONT_1_SWITCHmin, R_FRONT_1_SWITCHmax);
    R_FRONT_2_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_MAXangle, R_FRONT_2_SWITCHmin, R_FRONT_2_SWITCHmax);
    R_FRONT_3_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_MAXangle, R_FRONT_3_SWITCHmin, R_FRONT_3_SWITCHmax);
    R_FRONT_1.write(R_FRONT_1_angle);
    R_FRONT_2.write(R_FRONT_2_angle);
    R_FRONT_3.write(R_FRONT_3_angle);
  }
}

void LeftBackBWD() {
  
  // L_FRONT_1_angle, min.45,  std.90,  up.79,  switch.53/61,    max.98
  // L_BACK_1_angle,  min.135, std.90,  up.102, switch.128/121,  max.83
  // L_BACK_2_angle,  min.132, std.122, up.161, switch.130/128,  max.111
  // L_BACK_3_angle,  min.129, std.98,  up.100, switch.119/109,  max.75
  
  int L_FRONT_1_MAXangle = 98;
  int L_FRONT_1_MINangle = 45;
  int L_BACK_1_MAXangle = 83;
  int L_BACK_1_MINangle = 135;
  int L_BACK_1_UPangle = L_BACK_1_MAXangle + ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 2);
  int L_BACK_2_MAXangle = 111;
  int L_BACK_2_MINangle = 132;
  int L_BACK_2_UPangle = 161;
  int L_BACK_3_MAXangle = 75;
  int L_BACK_3_MINangle = 129;
  int L_BACK_3_UPangle = 100;
  
  int L_FRONT_1_SWITCHmin = L_FRONT_1_MINangle + ((L_FRONT_1_MAXangle - L_FRONT_1_MINangle) / 7);
  int L_FRONT_1_SWITCHmax = L_FRONT_1_SWITCHmin + ((L_FRONT_1_MAXangle - L_FRONT_1_MINangle) / 7);
  
  int L_BACK_1_SWITCHmin = L_BACK_1_MINangle - ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 7);
  int L_BACK_1_SWITCHmax = L_BACK_1_SWITCHmin - ((L_BACK_1_MINangle - L_BACK_1_MAXangle) / 7);
  
  int L_BACK_2_SWITCHmin = L_BACK_2_MINangle - ((L_BACK_2_MINangle - L_BACK_2_MAXangle) / 7);
  int L_BACK_2_SWITCHmax = L_BACK_2_SWITCHmin - ((L_BACK_2_MINangle - L_BACK_2_MAXangle) / 7);
  
  int L_BACK_3_SWITCHmin = L_BACK_3_MINangle - ((L_BACK_3_MINangle - L_BACK_3_MAXangle) / 7);
  int L_BACK_3_SWITCHmax = L_BACK_3_SWITCHmin - ((L_BACK_3_MINangle - L_BACK_3_MAXangle) / 7);
  
  if(L_FRONT_1_angle < L_FRONT_1_SWITCHmin && L_FRONT_1_angle > L_FRONT_1_PREVangle) {
    L_BACK_stepUP = 0;
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(L_FRONT_1_angle, L_FRONT_1_MINangle, L_FRONT_1_SWITCHmin, L_BACK_1_SWITCHmin, L_BACK_1_MINangle);
    L_BACK_2_angle = map(L_FRONT_1_angle, L_FRONT_1_MINangle, L_FRONT_1_SWITCHmin, L_BACK_2_SWITCHmin, L_BACK_2_MINangle);
    L_BACK_3_angle = map(L_FRONT_1_angle, L_FRONT_1_MINangle, L_FRONT_1_SWITCHmin, L_BACK_3_SWITCHmin, L_BACK_3_MINangle);
    L_BACK_1.write(L_BACK_1_angle);
    L_BACK_2.write(L_BACK_2_angle);
    L_BACK_3.write(L_BACK_3_angle);
  }
  else if(L_FRONT_1_angle >= L_FRONT_1_SWITCHmin && L_FRONT_1_angle <= L_FRONT_1_SWITCHmax && L_FRONT_1_angle > L_FRONT_1_PREVangle) {
    L_BACK_stepUP = 1;
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(L_FRONT_1_angle, L_FRONT_1_SWITCHmin, L_FRONT_1_SWITCHmax, L_BACK_1_MINangle, L_BACK_1_MAXangle);
    L_BACK_1.write(L_BACK_1_angle);
    if(L_BACK_1_angle >= L_BACK_1_UPangle) {
      L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_UPangle, L_BACK_2_MINangle, L_BACK_2_UPangle);
      L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_MINangle, L_BACK_1_UPangle, L_BACK_3_MINangle, L_BACK_3_UPangle);
      L_BACK_2.write(L_BACK_2_angle);
      L_BACK_3.write(L_BACK_3_angle);
    }
    else if(L_BACK_1_angle < L_BACK_1_UPangle) {
      L_BACK_2_angle = map(L_BACK_1_angle, L_BACK_1_UPangle, L_BACK_1_MAXangle, L_BACK_2_UPangle, L_BACK_2_MAXangle);
      L_BACK_3_angle = map(L_BACK_1_angle, L_BACK_1_UPangle, L_BACK_1_MAXangle, L_BACK_3_UPangle, L_BACK_3_MAXangle);
      L_BACK_2.write(L_BACK_2_angle);
      L_BACK_3.write(L_BACK_3_angle);
    }
  }
  else if(L_FRONT_1_angle > L_FRONT_1_SWITCHmax && L_FRONT_1_angle > L_FRONT_1_PREVangle) {
    L_BACK_stepUP = 0;
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(L_FRONT_1_angle, L_FRONT_1_SWITCHmax, L_FRONT_1_MAXangle, L_BACK_1_MAXangle, L_BACK_1_SWITCHmax);
    L_BACK_2_angle = map(L_FRONT_1_angle, L_FRONT_1_SWITCHmax, L_FRONT_1_MAXangle, L_BACK_2_MAXangle, L_BACK_2_SWITCHmax);
    L_BACK_3_angle = map(L_FRONT_1_angle, L_FRONT_1_SWITCHmax, L_FRONT_1_MAXangle, L_BACK_3_MAXangle, L_BACK_3_SWITCHmax);
    L_BACK_1.write(L_BACK_1_angle);
    L_BACK_2.write(L_BACK_2_angle);
    L_BACK_3.write(L_BACK_3_angle);
  }
  else if(L_FRONT_1_angle < L_FRONT_1_PREVangle) {
    L_BACK_stepUP = 0;
    L_BACK_1_PREVangle = L_BACK_1_angle;
    L_BACK_1_angle = map(L_FRONT_1_angle, L_FRONT_1_MAXangle, L_FRONT_1_MINangle, L_BACK_1_SWITCHmax, L_BACK_1_SWITCHmin);
    L_BACK_2_angle = map(L_FRONT_1_angle, L_FRONT_1_MAXangle, L_FRONT_1_MINangle, L_BACK_2_SWITCHmax, L_BACK_2_SWITCHmin);
    L_BACK_3_angle = map(L_FRONT_1_angle, L_FRONT_1_MAXangle, L_FRONT_1_MINangle, L_BACK_3_SWITCHmax, L_BACK_3_SWITCHmin);
    L_BACK_1.write(L_BACK_1_angle);
    L_BACK_2.write(L_BACK_2_angle);
    L_BACK_3.write(L_BACK_3_angle);
  }
}

void LeftFrontBWD() {
  
  // L_FRONT_1_angle, min.45, std.90, up.79, switch.63/80, max.98
  // L_FRONT_2_angle, min.49, std.59, up.19, switch.56/62, max.69
  // L_FRONT_3_angle, min.52, std.83, up.80, switch.70/88, max.106
  
  int L_FRONT_1_MAXangle = 98;
  int L_FRONT_1_MINangle = 45;
  int L_FRONT_1_UPangle = L_FRONT_1_MINangle + ((L_FRONT_1_MAXangle - L_FRONT_1_MINangle) / 2);
  int L_FRONT_2_MAXangle = 69;
  int L_FRONT_2_MINangle = 49;
  int L_FRONT_2_UPangle = 19;
  int L_FRONT_3_MAXangle = 106;
  int L_FRONT_3_MINangle = 52;
  int L_FRONT_3_UPangle = 80;
  
  if(L_FRONT_1_angle > L_FRONT_1_PREVangle && L_FRONT_1_angle <= L_FRONT_1_MAXangle) {
    L_FRONT_stepUP = 0;
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = L_FRONT_1_angle + 1;
    
    L_FRONT_1.write(L_FRONT_1_angle);
    L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_MINangle, L_FRONT_1_MAXangle, L_FRONT_2_MINangle, L_FRONT_2_MAXangle);
    L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_MINangle, L_FRONT_1_MAXangle, L_FRONT_3_MINangle, L_FRONT_3_MAXangle);
    L_FRONT_2.write(L_FRONT_2_angle);
    L_FRONT_3.write(L_FRONT_3_angle);    
    
    prevmicros = micros();
    while(micros() - prevmicros <= stepBwdSpeed) {
    }
  }
  
  else if(L_FRONT_1_angle > L_FRONT_1_PREVangle && L_FRONT_1_angle > L_FRONT_1_MAXangle){
    L_FRONT_stepUP = 1;
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = L_FRONT_1_MAXangle - 1;
  }
  
  else if(L_FRONT_1_angle < L_FRONT_1_PREVangle && L_FRONT_1_angle >= L_FRONT_1_MINangle){
    L_FRONT_stepUP = 1;
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = L_FRONT_1_angle - 1;
    
    L_FRONT_1.write(L_FRONT_1_angle);
    
    if(L_FRONT_1_angle <= L_FRONT_1_UPangle) {
      L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_UPangle, L_FRONT_1_MINangle, L_FRONT_2_UPangle, L_FRONT_2_MINangle);
      L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_UPangle, L_FRONT_1_MINangle, L_FRONT_3_UPangle, L_FRONT_3_MINangle);
      L_FRONT_2.write(L_FRONT_2_angle);
      L_FRONT_3.write(L_FRONT_3_angle);
    }
    else if(L_FRONT_1_angle > 79) {
      L_FRONT_2_angle = map(L_FRONT_1_angle, L_FRONT_1_MAXangle, L_FRONT_1_UPangle, L_FRONT_2_MAXangle, L_FRONT_2_UPangle);
      L_FRONT_3_angle = map(L_FRONT_1_angle, L_FRONT_1_MAXangle, L_FRONT_1_UPangle, L_FRONT_3_MAXangle, L_FRONT_3_UPangle);
      L_FRONT_2.write(L_FRONT_2_angle);
      L_FRONT_3.write(L_FRONT_3_angle);
    }
    
    prevmicros = micros();
    while(micros() - prevmicros <= stepFwdSpeed) {
    }
  }
  
  else if(L_FRONT_1_angle < L_FRONT_1_PREVangle && L_FRONT_1_angle < L_FRONT_1_MINangle){
    L_FRONT_stepUP = 0;
    L_FRONT_1_PREVangle = L_FRONT_1_angle;
    L_FRONT_1_angle = L_FRONT_1_MINangle + 1;
  }
}

/* ------------------------------- END OF WALK BACKWARD ----------------------------------------------*/
