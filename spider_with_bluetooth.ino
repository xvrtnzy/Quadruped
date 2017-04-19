#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

SoftwareSerial I2CBT(10,11);//定義PIN10及PIN11分別為RX及TX腳位
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150
#define SERVOMAX  560

uint8_t servonum = 0;
int i,j,k;

int RB1 = 0;
int RB2 = 1;
int RB3 = 2;
int LB1 = 3;
int LB2 = 4;
int LB3 = 5;
int LF1 = 6;
int LF2 = 7;
int LF3 = 8;
int RF1 = 9;
int RF2 = 10;
int RF3 = 11;

void setup() {
  Serial.begin(9600); //Arduino起始鮑率：9600
  I2CBT.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  digitalWrite(3,LOW);
  digitalWrite(4,HIGH);
  
  start();
  delay(1000);
  stand();
  delay(1000);
}

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}
void deg(int k,int a){
  a = map(a,0,180,150,560);
  pwm.setPWM(k,0,a);
}

void degr(int k,int a){
  a = map(a,0,180,150,560);
  pwm.setPWM(k,0,a);
}
void degl(int k,int a){
  a = map(a,180,0,150,560);
  pwm.setPWM(k,0,a);
}

void loop() {
  byte cmmd[20];
  int insize;	
  while(1){
    digitalWrite(3,LOW);
    if ((insize=(I2CBT.available()))>0){  //讀取藍牙訊息
      Serial.print("input size = ");
      Serial.println(insize);
      for (int i=0; i<insize; i++){
        Serial.print(cmmd[i]=char(I2CBT.read()));
        Serial.print(" ");
      }
    }
    switch (cmmd[0]) { //讀取第一個字
    
    case 97: //97為"a"的ASCII CODE  此為測試用
      digitalWrite(13,HIGH);  //點亮LED
      break;

    case 98://98為"b"的ASCII CODE
      Serial.println("Get b");
      digitalWrite(13,LOW);   //熄滅LED此為測試用
      break;
    case 102://102為"f"的ASCII CODE
      FWD();
      break;
    case 115://115為"s"的ASCII CODE
      stand();
      break;
    case 107://107為"k"的ASCII CODE b:98
      BWD();
      break;
    case 114://114為"r"的ASCII CODE
      fire2();
      break;
    case 108://108為"l"的ASCII CODE
      fire();
      break;
    } //Switch
  } //while
}//loop
void fire(){
   int buttonState = 0;
   buttonState = digitalRead(5);
   digitalWrite(4,HIGH);
   
   digitalWrite(3,HIGH);
   delay(30);
   for(int i=0;i<10;i++){
     if(buttonState == HIGH){
       digitalWrite(3,LOW);
       delay(100);
       break;
     }
   }
   //delay(130);
   //digitalWrite(3,LOW);
 }
void fire2(){
  digitalWrite(3,HIGH);
  delay(90);
  digitalWrite(3,LOW);
  delay(1000);
}
void lay(){
  for(i = 0;i < 12;i++)
    deg(i,90);
}

void start(){
  degr(RB1,90);
  degr(RB2,0);
  degr(RB3,90);
 
  degl(LB1,90);
  degl(LB2,0);
  degl(LB3,90);
  
  degl(LF1,90);
  degl(LF2, 0);
  degl(LF3,90);

  degr(RF1,90);
  degr(RF2,0);
  degr(RF3,90);  
}
void stand(){
 // pwm.setPWM(RB1, 0, 457.5); //RB1
 // pwm.setPWM(RB2, 0, 252.5); //RB2
 // pwm.setPWM(RB3, 0, 252.5); //RB3 
  //pwm.setPWM(LB1, 0, 252.5); //LB1
  //pwm.setPWM(LB2, 0, 457.5); //LB2
  //pwm.setPWM(LB3, 0, 457.5); //LB3
  //pwm.setPWM(LF1, 0, 252.5); //LF1
  //pwm.setPWM(LF2, 0, 457.5); //LF2
  //pwm.setPWM(LF3, 0, 252.5); //LF3
  //pwm.setPWM(RF1, 0, 457.5); //RF1
  //pwm.setPWM(RF2, 0, 252.5); //RF2
  //pwm.setPWM(RF3, 0, 457.5); //RF3
  degr(RB1,135);
  degr(RB2,35);
  degr(RB3,45);
  degl(LB1,125);
  degl(LB2,35);
  degl(LB3,45);
  degl(LF1,125);
  degl(LF2,35);
  degl(LF3,135);
  degr(RF1,135);
  degr(RF2,35);
  degr(RF3,135);
}

void RBFWD_1(){
  /*pwm.setPWM(0, 0, 503); //RB1  135 -> 165
  pwm.setPWM(1, 0, 298); //RB2  35->65
  pwm.setPWM(2, 0, 298); //RB3  45->63
  */
  degr(RB1,165);
  degr(RB2,65);
  degr(RB3,70);
}

void RBFWD_2(){
  /*pwm.setPWM(0, 0, 457.5); //RB1 165 -> 130
  pwm.setPWM(1, 0, 252.5); //RB2 55 -> 25
  */
  degr(RB1,130);
  degr(RB2,35);
}

void RBFWD_3(){
  //pwm.setPWM(2, 0, 207); //RB2 45 -> 27
  degr(RB3,20);
}

void RBFWD(){     //3畾萄???
  RBFWD_1();
  delay(250);
  //-------------------------------------------------------
  RBFWD_2();
  delay(250);
  //-------------------------------------------------------------
  RBFWD_3();
  delay(250);
}

void LFFWD_1(){
  degl(LF1,155);  //45->27
  degl(LF2,65);  //45->15
  degl(LF3,160);   //45->27
}
void LFFWD_2(){
  degl(LF1,130);  //27->45
  degl(LF2,35);  //117->135
}

void LFFWD_3(){
  degl(LF3,110);//27->63
}

void LFFWD(){
  LFFWD_1();
  delay(150);
  LFFWD_2();
  delay(150);
  LFFWD_3();
  delay(150);
}

void LBFWD_2(){
  degl(LB1,155 );//125 -> 155
  degl(LB2,70);//35 -> 65
  degl(LB3,70);//45 -> 
}
void LBFWD_3(){
  degl(LB1,130); //27->45
  degl(LB2,35); //65->35
}
void LBFWD_1(){
  degl(LB3,20);//117->153
}

void LBFWD(){
  LBFWD_1();
  delay(150);
  LBFWD_2();
  delay(150);
  LBFWD_3();
  delay(150);
}

void RFFWD_2(){
  degr(RF1,165);//135 -> 165
  degr(RF2,65);//35->65
  degr(RF3,160);//135->153
}

void RFFWD_3(){
  degr(RF1,130); //165->135
  degr(RF2,35); //63->45
}

void RFFWD_1(){
  degr(RF3,110);  // 153->117
}
void RFFWD(){
  RFFWD_1();
  delay(150);
  RFFWD_2();
  delay(150);
  RFFWD_3();
  delay(150);
}

void FWD(){  
  RBFWD_1();
  LFFWD_1();
  LBFWD_1();
  RFFWD_1();
  delay(250);
  
  RBFWD_2();
  LFFWD_2();
  LBFWD_2();
  RFFWD_2();
  delay(250);
  
  RBFWD_3();
  LFFWD_3(); 
  LBFWD_3();
  RFFWD_3();
  delay(250);
}

//-------------------------------------------------------

void RBBWD_1(){
  /*pwm.setPWM(0, 0, 503); //RB1  135 -> 165
  pwm.setPWM(1, 0, 298); //RB2  35->65
  pwm.setPWM(2, 0, 298); //RB3  45->63
  */
  degr(RB1,165);
  degr(RB2,65);
  degr(RB3,20);
}

void RBBWD_2(){
  /*pwm.setPWM(0, 0, 457.5); //RB1 165 -> 130
  pwm.setPWM(1, 0, 252.5); //RB2 55 -> 25
  */
  degr(RB1,130);
  degr(RB2,35);
}


void RBBWD_3(){
  //pwm.setPWM(2, 0, 207); //RB2 45 -> 27
  degr(RB3,70);
}

void RBBWD(){     //3畾萄???
  RBBWD_1();
  delay(250);
  //-------------------------------------------------------
  RBBWD_2();
  delay(250);
  //-------------------------------------------------------------
  RBBWD_3();
  delay(250);
}

void LFBWD_1(){
  degl(LF1,155);  //45->27
  degl(LF2,65);  //45->15
  degl(LF3,110);   //45->27
}
void LFBWD_2(){
  degl(LF1,130);  //27->45
  degl(LF2,35);  //117->135
}

void LFBWD_3(){
  degl(LF3,160);//27->63
}

void LFBWD(){
  LFBWD_1();
  delay(150);
  LFBWD_2();
  delay(150);
  LFBWD_3();
  delay(150);
}

void LBBWD_2(){
  degl(LB1,155 );//125 -> 155
  degl(LB2,70);//35 -> 65
  degl(LB3,20);//45 -> 
}
void LBBWD_3(){
  degl(LB1,130); //27->45
  degl(LB2,35); //65->35
}
void LBBWD_1(){
  degl(LB3,70);//117->153
}

void LBBWD(){
  LBBWD_1();
  delay(150);
  LBBWD_2();
  delay(150);
  LBBWD_3();
  delay(150);
}

void RFBWD_2(){
  degr(RF1,165);//135 -> 165
  degr(RF2,65);//35->65
  degr(RF3,110);//135->153
}

void RFBWD_3(){
  degr(RF1,130); //165->135
  degr(RF2,35); //63->45
}

void RFBWD_1(){
  degr(RF3,160);  // 153->117
}
void RFBWD(){
  RFBWD_1();
  delay(150);
  RFBWD_2();
  delay(150);
  RFBWD_3();
  delay(150);
}

void BWD(){  
  RBBWD_1();
  LFBWD_1();
  LBBWD_1();
  RFBWD_1();
  delay(250);
  
  RBBWD_2();
  LFBWD_2();
  LBBWD_2();
  RFBWD_2();
  delay(250);
  
  RBBWD_3();
  LFBWD_3(); 
  LBBWD_3();
  RFBWD_3();
  delay(250);
}
