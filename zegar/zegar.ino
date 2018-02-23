/////%%%%%%%%%%%%BIBLIOTEKI%%%%%%%%%%%%%%%%\\\\\\
#include <DS1307RTC.h>
#include <Timers.h>
#include <DS1307RTC.h>
#include <TM1637Display.h>
#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
// PRZERWANIA
Timers <4> przerwanie;
// DEFINICJA PINÓW
#define P1 3 
#define OK 10
#define minus 9
#define plus 2
#define CLK 11  //Clock pin          
#define DIO 12  //Data pin
/// ZMIENNE GLOBALNE
tmElements_t Clock;                // stworzenie obiektu Clock
TM1637Display display(CLK, DIO);  // stworzenie obiektu klasy TM1637Display
int EXIT =0;
int minutes=0;
int flaga=0;
int godzina;
int minuta;
int Hours=7; int Minutes=0;
int budzflag=0;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
uint8_t nap_stoper[] = {
  SEG_A|SEG_F|SEG_G|SEG_C|SEG_D,
  SEG_F|SEG_G|SEG_E|SEG_D,
  SEG_B|SEG_A|SEG_F|SEG_E|SEG_C|SEG_D,
  SEG_A|SEG_F|SEG_B|SEG_G|SEG_E,
};
uint8_t nap_minutnik[] = {
  SEG_A|SEG_F|SEG_E|SEG_B|SEG_C,
  SEG_E,
  SEG_E|SEG_G|SEG_C,
  SEG_E|SEG_C|SEG_D,
};

uint8_t nap_budzik[] = {
  SEG_F|SEG_E|SEG_D|SEG_C|SEG_G,
  SEG_E|SEG_C|SEG_D,
  SEG_B|SEG_C|SEG_D|SEG_E|SEG_G,
  SEG_A|SEG_B|SEG_G|SEG_E|SEG_D,
};

uint8_t nap_wlacz[] = {
  SEG_F|SEG_E|SEG_D|SEG_C|SEG_B,
  SEG_F|SEG_E|SEG_D,
  0,
  0,
};
uint8_t nap_wylacz[] = {
  SEG_F|SEG_E|SEG_D|SEG_C|SEG_B,
  SEG_F|SEG_G|SEG_B|SEG_C|SEG_D,
  SEG_F|SEG_E|SEG_D,
  0,
};
uint8_t clearMe[]={
  0,0,0,0 
};



void alarm(){
  if(budzflag==1&&Clock.Hour==Hours&&Clock.Minute==Minutes){
    budzflag=0;
    while(digitalRead(OK)!=LOW){
      //tone(A3, 4300);  
      display.showNumberDec(Minutes,true,2,2);
      display.showNumberDecEx(Hours,(0x80 >> 3),true,2,0);
      delay(200); tone(A3, 3500);
      delay(200); tone(A3, 4300);  
      display.setSegments(clearMe);
      delay(500);
    }  
}}
void dzwiek(){
  while(digitalRead(OK)!=LOW){
  tone(A3, 5);  
  delay(150);  
  //tone(A3, 3500);   
  noTone(A3);
  delay(150);
  if(digitalRead(OK)==LOW) {noTone(A3);}
  }
}
/////////////////////////////////////////////////////////////////////////////////
void zegar(){
  if(Clock.Minute<10){Clock.Hour=Clock.Hour*10;}
  String myTime = String(Clock.Hour)+String(Clock.Minute);
  int myTimeInt = myTime.toInt();
  if(flaga==0){display.showNumberDecEx(myTimeInt, (0x80 >> 1));flaga=1;}
  else if(flaga==1){display.showNumberDec(myTimeInt); flaga=0;}}
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
 void budzik(){
 int flag=0; int flag1=0; int flag2=0; int flag3=0; int flagH=0;
 int czasalarm=0;
  display.setSegments(clearMe); delay(200);
  display.setSegments(nap_wlacz,4,0);
   while(flag!=1){
    if(digitalRead(P1)==LOW&&flag1==0){delay(200); flag1=1;display.setSegments(nap_wylacz,4,0);}
    if(digitalRead(P1)==LOW&&flag1==1){delay(200); flag1=0;display.setSegments(nap_wlacz,4,0);}
    if(digitalRead(OK)==LOW) flag=1;
    if(digitalRead(plus)==LOW&&flag1==1) {delay(200); budzflag=0;display.setSegments(clearMe);
    delay(500);display.setSegments(nap_wylacz,4,0);delay(500);display.setSegments(clearMe);delay(500);
    display.setSegments(nap_wylacz,4,0); flag=1;}
    if(digitalRead(plus)==LOW&&flag1==0){
      delay(200); 
      while(flag2==0){
        if(digitalRead(plus)==LOW&&flag3==0){delay(200); Hours++;}
        if(digitalRead(minus)==LOW&&flag3==0){delay(200); Hours--;}
        if(digitalRead(plus)==LOW&&flag3==1){delay(200); Minutes++;}
        if(digitalRead(minus)==LOW&&flag3==1){delay(200); Minutes--;}
        if(digitalRead(P1)==LOW&&flag3==0){delay(200);flag3=1;}
        if(digitalRead(P1)==LOW&&flag3==1){delay(200);flag3=0;}
        if(Hours>=24) {Hours=0;}
        if(Hours<0) {Hours=23;}
        if(Minutes>59) {Minutes=0;Hours++;} if(Minutes<0) {Minutes=59;Hours--;}
        //if(Minutes<10&&flagH==0) { Hours=Hours*10; flagH=1;}
        //if(Minutes>=10&&flagH==1) {Hours=Hours/10; flagH=0;}
        //String czasalarmS = String(Hours)+String(Minutes);
        //czasalarm=czasalarmS.toInt();
        //display.showNumberDecEx(czasalarm,(0x80 >> 1),true,4,0);
        display.showNumberDec(Minutes,true,2,2);
        display.showNumberDecEx(Hours,(0x80 >> 3),true,2,0); 
        if(digitalRead(OK)==LOW) {budzflag=1;display.setSegments(clearMe);
          delay(500);display.setSegments(nap_wlacz,4,0);delay(500);display.setSegments(clearMe);delay(500);
          display.setSegments(nap_wlacz,4,0);flag2=1;flag=1;}
    }
    }
   }
   
  }
 
 /////////////////////////////////////
 void stoper(){
  int tmsec = 0;int tmmin =0;int tmsum=0; int flag=0; int flag2=0; int flag3=0;
display.showNumberDec(0,true, 4, 0);
delay(300);
  while(digitalRead(OK)!=LOW){
   if(digitalRead(P1)==LOW) flag=1;
   if(flag==1) {display.showNumberDecEx(tmsum,(0x80 >> 1)); delay(1000); tmsec++; }
   if(tmsec>59) { tmmin++;tmsec=0;}
   if(tmmin>=1&&tmsec<10&&flag2==0) {tmmin=tmmin*10;flag2=1;}
   if(tmsec>=10&&flag2==1) {tmmin=tmmin/10; flag2=0;}
   String tmsumstring = String(tmmin)+String(tmsec);
   tmsum=tmsumstring.toInt();
   if(digitalRead(plus)==LOW&&flag3==0){flag=0;flag3=1;Serial.println("tutaj działam");Serial.println(flag);delay(300);}
   if(digitalRead(plus)==LOW&&flag3==1){flag=1;flag3=0;Serial.println(flag);}
   if(digitalRead(minus)==LOW){flag=0;display.showNumberDec(0,true, 4, 0);tmsec=0;tmmin=0;tmsum=0;}
  }}
  ////////////////////////////////////////////////////////////////////////
 void minutnik() {
  display.showNumberDec(0,true, 4, 0);    
    int Mins =1;
    int Minsec =0;
    int flagMins=0;
    int flag2=0;
    delay(200);
  while(digitalRead(OK)!=LOW){
    if(digitalRead(plus)==LOW){delay(200);Mins=Mins+1;Serial.println("+");Serial.println(Mins);}
    if(Mins>60) Mins=60;
    if(digitalRead(minus)==LOW){delay(200);Mins=Mins-1;Serial.println("-");Serial.println(Mins);}
    if(Mins<1) {Mins=1;}
    display.showNumberDecEx(Mins,(0x80 >> 1));
    if(digitalRead(P1)==LOW) {flagMins=1;Mins=Mins-1;Minsec=60;}
    while(flagMins==1){
      delay(1000);
      Minsec--;
      if(Minsec<=0&&Mins>0){Mins=Mins-1; Minsec=59;}
      if(Mins<0) {Mins=0;}
      if(Minsec<=0){Minsec=0;}
      if(Mins>=1&&Minsec<10&&flag2==0) {Mins=Mins*10;flag2=1;}
      if(Minsec>=10&&flag2==1) {Mins=Mins/10; flag2=0;}
      String Minsum = String(Mins)+String(Minsec);
      int Minsumint = Minsum.toInt();
      display.showNumberDecEx(Minsumint,(0x80 >> 1));
      if(digitalRead(OK)==LOW){ flagMins=0;Serial.println("test-ok");}
      if(Mins==0&&Minsec==0) dzwiek();
  }
  }}

void panel(){ 
if(digitalRead(P1)==LOW){
delay(150);
int f=0;
int number=1;
  while(EXIT!=1){
  if(digitalRead(P1)==LOW) {delay(200); number=number+1;}
  if(number>=4){ number=1;}
  if(digitalRead(plus)==LOW&&number==1) budzik();
  if(digitalRead(plus)==LOW&&number==2) stoper();
  if(digitalRead(plus)==LOW&&number==3) minutnik();
  if(digitalRead(OK)==LOW) EXIT=1;
  if(number==1)display.setSegments(nap_budzik,4,0);
  if(number==2)display.setSegments(nap_stoper,4,0);  
  if(number==3)display.setSegments(nap_minutnik,4,0); 
 } }}
  
void setup(){
  //Wire.begin();
  Serial.begin(9600);
  display.setBrightness(0x0f);
  pinMode(A3,OUTPUT);
  pinMode(minus,INPUT_PULLUP);
  pinMode(OK,INPUT_PULLUP);
  pinMode(plus,INPUT_PULLUP);
  pinMode(P1,INPUT_PULLUP);
  analogWrite(A3,0);

 przerwanie.attach(2,1000,zegar);
 przerwanie.attach(3,1001, alarm);
 przerwanie.attach(1,133,panel);
}
//////////////////////////////////////////////////////////////////////////////

void loop(){
  EXIT=0;
  RTC.read(Clock);
  przerwanie.process(); 
  noTone(A3);
}
 
