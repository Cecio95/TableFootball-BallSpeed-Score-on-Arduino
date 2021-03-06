#include <ClickButton.h>
#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal.h>
const int E4 = 330;
const int F4 = 349;
const int G4 = 392;
const int a4 = 440;
const int Gd = 415;
const int B4 = 494;
const int C5 = 523;
const int D5 = 587;
const int E5 = 659;
const int F5 = 698;
const int E6 = 1319;
const int G6 = 1568;
const int a6 = 1760;
const int As6 = 1865;
const int B6 = 1976;
const int C7 = 2093;
const int D7 = 2349;
const int E7 = 2637;
const int F7 = 2794;
const int G7 = 3136;
const int a7 = 3520;

//sensori IR
volatile unsigned long time0R=0;
volatile int stateR=0;
volatile unsigned long time1R=0;
volatile unsigned long time0B=0;
volatile int stateB=0;
volatile unsigned long time1B=0;
int sensReadR=2; //digital input 3 - interrupt - red
int sensReadB=3; //digital input 2 - interrupt - blue
long timectrR=0;
long timectrB=0;
float timedifR =0.0;
float timedifB =0.0;
float velR=0.0;
float velmaxR=0.0;
float velgoalR=0.0;
float velB=0.0;
float velmaxB=0.0;
float velgoalB=0.0;

const int ledPin1 = 8; //blue
const int ledPin2 = 9; //red
const int buzzer = 10;
const int BP1 = 6; //pin tasto1 - red
const int BP2 = 7; //pin tasto2 - blue
LiquidCrystal_I2C lcd(0x27,16,2); //dichiaro l'indirizzo e la dimensione del display
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //VSS:ground;VDD:5v;V0:trim;rs:12;rw:ground;e:11;d4:5;d5:4;d6:3;d7:2
ClickButton button1(BP1, LOW, CLICKBTN_PULLUP);
ClickButton button2(BP2, LOW, CLICKBTN_PULLUP);
int counter = 0;
int a = 0;
int b = 0;
int start = 0;
int uno = 0;
int due = 0;
String V1 = "";
String V2 = "";

void setup() {
   // Setup button timers (all in milliseconds / ms)
  // (These are default if not set, but changeable for convenience)
  button1.debounceTime   = 10;   // Debounce timer in ms
  button1.multiclickTime = 300;  // Time limit for multi clicks
  button1.longClickTime  = 500; // time until "held-down clicks" register

  button2.debounceTime   = 10;   // Debounce timer in ms
  button2.multiclickTime = 300;  // Time limit for multi clicks
  button2.longClickTime  = 500; // time until "held-down clicks" register

  pinMode(buzzer, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  lcd.init();
  lcd.backlight();
  //lcd.begin(16,2);
  lcd.print(" Calcio Balilla");
  lcd.setCursor(4,1);
  lcd.print("CasaGere");
  Mario();
  lcd.clear();
  lcd.print("Initializing ir");
  //delay(2000);
  //lcd.clear();
  attachInterrupt(digitalPinToInterrupt(sensReadR), letturaR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sensReadB), letturaB, CHANGE);
  delay(2000);
  lcd.clear();
  //delay(50); 
}

void loop() {

  while(start==0){

    if(time0R>timectrR){
      timedifR=time1R - time0R;
      velR=(0.033/(timedifR/1000000))*3.60;
    }
    if(time0B>timectrB){
      timedifB=time1B - time0B;
      velB=(0.033/(timedifB/1000000))*3.60;
    }
    
    button1.Update();
    lcd.clear();
    //delay(200);
    lcd.print(" Tieni premuto");
    lcd.setCursor(2,1);
    lcd.print("per giocare");
    delay(1500);
    lcd.clear();
    lcd.print("    Goal di ");
    lcd.setCursor(0,1);
    lcd.print(" riscaldamento!");
    delay(1500);
        
    if(button1.clicks!=0) a = button1.clicks;
    if(a == -1 || velR!=0 || velB!=0){  
      lcd.clear();
      start=1;
      resetvar();
      delay(400);
      lcd.print("   Iniziamo!");
      //StarWars();
      delay(1500);
      lcd.clear();
      printscreen();
      }
    }
  
  //calcolo velocità rossi
  if(time0R>timectrR && time0R-timectrR>2000000){
    timedifR=time1R - time0R;
    velR=(0.0330/(timedifR/1000000))*3.6000;
    velgoalR=velR;
    timectrR=time0R;
    if(velR>velmaxR)
      velmaxR=velR;
  }
  //calcolo velocità blu
  if(time0B>timectrB && time0B-timectrB>2000000){
    timedifB=time1B - time0B;
    velB=(0.033/(timedifB/1000000))*3.60;
    velgoalB=velB;
    timectrB=time0B;
    if(velB>velmaxB)
      velmaxB=velB;
  }
  
  //delay(2000);

  if(uno==due){
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin1, LOW);
  }
  else{
    if(uno>due){
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
    }
    else{
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin1, HIGH);
    }
  }
  button1.Update();
  button2.Update();
  delay(50);
  
  if(button1.clicks == -3 && button2.clicks == -3){
    start=0;
  }
  
  if(button1.clicks!=0) a = button1.clicks;

  if(uno>0 && a == 2){
    uno--;
    printscreen();
    }
  if((a == 1 || velR>0) && uno<9 && V1.equals("") && V2.equals("")){ 
    if(a!=1)
      velgoalRed(velgoalR);    
    velR=0;
    uno++;
    printscreen();
    }
  else{ 
    if((a == 1 || velR>0) && (uno+due)<18 && uno==9){ 
      if(a!=1)
        velgoalRed(velgoalR);      
      velR=0;
      V2=":(";
      V1="Vittoria!";
    }
    if((a == 1 || velR>0) && uno+due>=18 && V1.equals("") && !V2.equals("Adv")){ 
      if(a!=1)
        velgoalRed(velgoalR);      
      velR=0;
      V1="Adv";
      uno++;
      printscreen();
    }
    else{
      if((a == 1 || velR>0) && uno+due>=18 && V1.equals("") && V2.equals("Adv")){ 
        if(a!=1)
          velgoalRed(velgoalR);       
        velR=0;
        V1="";
        V2="";
        uno++;
        printscreen();;
      }
      else{
        if((a == 1 || velR>0) && uno+due>18 && V1.equals("Adv") && !V2.equals("Adv")){ 
          if(a!=1)
            velgoalRed(velgoalR);        
        velR=0;
        V2=":(";
        V1="Vittoria!";
        uno++;
      }
     }
    }
   }
    a=0;
          
  
  if(button2.clicks!=0) b = button2.clicks;
  
  if(due>0 && b == 2){
    due--;
    printscreen();
    }
  if((b == 1 || velB>0) && due<9 && V1.equals("") && V2.equals("")){ 
    if(b!=1)
      velgoalBlu(velgoalB);    
    velB=0;
    due++;
    printscreen();
    }
  else{ 
    if((b == 1 || velB>0) && (uno+due)<18 && due==9){ 
      if(b!=1)
        velgoalBlu(velgoalB);      
      velB=0;
      V1=":(";
      V2="Vittoria!";
    }   
    if((b == 1 || velB>0) && (uno+due)>=18 && V2.equals("") && !V1.equals("Adv")){ //
       if(b!=1)
        velgoalBlu(velgoalB);       
       velB=0;
       V2="Adv";
       due++;
       printscreen();
    }
    else{
      if((b == 1 || velB>0) && (uno+due)>=18 && V2.equals("") && V1.equals("Adv")){ 
        if(b!=1)
          velgoalBlu(velgoalB);        
        velB=0;
        V2="";
        V1="";
        due++;
        printscreen();
      }
      else{
        if((b == 1 || velB>0) && (uno+due)>18 && V2.equals("Adv") && !V1.equals("Adv")){ 
         if(b!=1)
          velgoalBlu(velgoalB);         
         velB=0;
         V1=":(";
         V2="Vittoria!";
         due++;
        }
      }
    }
   }
    b=0;

  if(V1.equals("Vittoria!") || V2.equals("Vittoria!")){
      lcd.clear();
      lcd.print("  Complimenti");
      if(V2.equals("Vittoria!")){
        start=0;
        lcd.setCursor(6,1);
        lcd.print("Blu!");
        pirates();
        delay(2000);
      }
      else{
        start=0;
        lcd.setCursor(5,1);
        lcd.print("Rossi!");
        pirates();
        delay(2000);
      }
    lcd.clear();
    lcd.print("Velocita'massima");
    lcd.setCursor(0,1);
    lcd.print("raggiunta dai: ");
    delay(1000);
    lcd.clear();
    if(velmaxR>velmaxB){
     lcd.print("Rossi!");
     lcd.setCursor(0,1);
     lcd.print(velmaxR);
     lcd.print(" km/h"); 
     delay(3000);
    }
    else{
     lcd.print("Blu!");
     lcd.setCursor(0,1);
     lcd.print(velmaxB);
     lcd.print(" km/h");  
     delay(3000);
    }
   }
}

void printscreen(){
  lcd.clear();
  lcd.setCursor(2,0); //position the LCD cursor (col,row)
  lcd.print("Rossi"); // V1 button1
  lcd.setCursor(11,0);
  lcd.print("Blu"); // V2 button2
  //delay(2000);
  lcd.setCursor(4,1);
  if(uno<=9 && due<=9)
    lcd.print(uno);
  else{
    lcd.setCursor(3,1);
    lcd.print(V1);
  }
  lcd.setCursor(12,1);
  if(due<=9 && uno<=9)
    lcd.print(due);
  else{
    lcd.setCursor(11,1);
    lcd.print(V2);
  }
}

void letturaR(){
  if(stateR%2==0){
   time0R=micros();
   stateR++;
  }
  else{
    time1R=micros();
    stateR++;
  } 
}

void letturaB(){
  if(stateB%2==0){
   time0B=micros();
   stateB++;
  }
  else{
    time1B=micros();
    stateB++;
  } 
}

void velgoalRed(float x){
  if(x<2.0){
    lcd.clear();
    lcd.print("  Troppo lento");
    lcd.setCursor(0,1);
    lcd.print("   TARTARUGA!");
    delay(3000);
    lcd.clear();
  }
  else{
    lcd.clear();
    lcd.print("Velocita' tiro:");
    lcd.setCursor(0,1);
    lcd.print(x);
    lcd.print(" km/h");
    delay(3000);
    lcd.clear();
  }
}

void velgoalBlu(float x){
  if(x<2.0){
    lcd.clear();
    lcd.print("  Troppo lento");
    lcd.setCursor(0,1);
    lcd.print("   TARTARUGA!");
    delay(3000);
    lcd.clear();
  }
    else{
    lcd.clear();
    lcd.print("Velocita' tiro:");
    lcd.setCursor(0,1);
    lcd.print(x);
    lcd.print(" km/h");
    delay(3000);
    lcd.clear();
    }
}

void resetvar(){
     V1="";
     V2="";
     uno=0;
     due=0;
     a=0;
     b=0;
     time0B=0;
     time1B=0;
     timedifB=0.0;
     time0R=0;
     time1R=0;
     timedifR=0.0;
     stateR=0;
     velR=0.0;
     velgoalR=0.0;
     stateB=0;
     velB=0.0;
     velgoalB=0.0;
     velmaxR=0.0;
     velmaxB=0.0;
     timectrR=0;
     timectrB=0;
}

void beep(int nota, int durata){
  tone(buzzer, nota, durata); //play
 
  //led alternati
  if(counter%2==0){
    digitalWrite(ledPin1, HIGH);
    delay(durata);
    digitalWrite(ledPin1, LOW);
  }
  else{
    digitalWrite(ledPin2, HIGH);
    delay(durata);
    digitalWrite(ledPin2, LOW);
  }
  noTone(buzzer); //stacco tra una nota e l'altra
  delay(50);
  
  counter++;
}
  
void pirates(){
 // for(int  i=0;i<2;i++){
    beep(E4, 125);
    beep(G4, 125);
    beep(a4, 250);
    beep(a4, 125);
    delay(125);
    beep(a4, 125);
    beep(B4, 125);
    beep(C5, 250);
    beep(C5, 125);
    delay(125);
    beep(C5, 125);
    beep(D5, 125);
    beep(B4, 250);
    beep(B4, 125);
    delay(125);
    beep(a4, 125);
    beep(G4, 125);
    beep(a4, 375);
    delay(250);
 // }
  beep(E4, 125);
  beep(G4, 125);
  beep(a4, 250);
  beep(a4, 125);
  delay(125);
  beep(a4, 125);
  beep(C5, 125);
  beep(D5, 250);
  beep(D5, 125);
  delay(125);
  beep(D5, 125);
  beep(E5, 125);
  beep(F5, 250);
  beep(F5, 125);
  delay(125);
  beep(E5, 125);
  beep(D5, 125);
  beep(E5, 125);
  beep(a4, 250);
  delay(125);

  beep(a4, 125);
  beep(B4, 125);
  beep(C5, 250);
  beep(C5, 125);
  delay(125);
  beep(D5, 250);
  beep(E5, 125);
  beep(a4, 250);
  delay(250);
  beep(a4, 125);
  beep(C5, 125);
  beep(B4, 250);
  beep(B4, 125);
  delay(125);
  beep(C5, 125);
  beep(B4, 125);
  beep(a4, 300);
  delay(500); 
}

void StarWars(){
  beep(a4, 500);
  beep(a4, 500);    
  beep(a4, 500);
  beep(F4, 350);
  beep(C5, 150);  
  beep(a4, 500);
  beep(F4, 350);
  beep(C5, 150);
  beep(a4, 650);
 
  delay(500);
 
  beep(E5, 500);
  beep(E5, 500);
  beep(E5, 500);  
  beep(F5, 350);
  beep(C5, 150);
  beep(Gd, 500);
  beep(F4, 350);
  beep(C5, 150);
  beep(a4, 650);
 
  delay(500);
}

void Mario(){
  beep(E7, 48);
  beep(E7, 48);
  delay(48);
  beep(E7, 48);
  delay(48);
  beep(C7, 48);
  beep(E7, 48);
  delay(48);
  beep(G7, 48);
  delay(144);
  beep(G6, 48);
  delay(144);

  beep(C7, 48);
  delay(96);
  beep(G6, 48);
  delay(96);
  beep(E6, 48);
  delay(96);
  beep(a6, 48);
  delay(48);
  beep(B6,48);
  delay(48);
  beep(As6, 48);
  beep(a6, 48);
  delay(48);

  beep(G6, 36);
  beep(E7, 36);
  beep(G7, 36);
  beep(a7, 48);
  delay(48);
  beep(F7, 48);
  beep(G7, 48);
  delay(48);
  beep(E7, 48);
  delay(48);
  beep(C7, 48);
  beep(D7, 48);
  beep(B6, 48);
  delay(96);

  beep(C7, 48);
  delay(96);
  beep(G6, 48);
  delay(96);
  beep(E6, 48);
  delay(96);
  beep(a6, 48);
  delay(48);
  beep(B6,48);
  delay(48);
  beep(As6, 48);
  beep(a6, 48);
  delay(48);

  beep(G6, 36);
  beep(E7, 36);
  beep(G7, 36);
  beep(a7, 48);
  delay(48);
  beep(F7, 48);
  beep(G7, 48);
  delay(48);
  beep(E7, 48);
  delay(48);
  beep(C7, 48);
  beep(D7, 48);
  beep(B6, 48);
  beep(C7, 60);
  delay(96);
}
 

  
