#include <hx711.h> //peddle 1
#include "Timer.h" //rotor  2
#include <TimedAction.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
float data1,data2;
int relayPin = 13;
/////peddle sensor starts
Hx711 scale(A2, A3);
float pressure;
/////peddle sensor ends
String state;
/////rpm starts
volatile float rev2 = 0;
void iterate2()
{
  rev2 = rev2 + 1.0;
}
/////rpm ends
TimedAction timedpeddle = TimedAction(127,peddle);
TimedAction timedrpm = TimedAction(97,rpm);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
         //initializing pin Analog 0 to INPUT
  attachInterrupt(digitalPinToInterrupt(2), iterate2, RISING);
  lcd.begin(16, 4);
  pinMode(relayPin , OUTPUT);
  digitalWrite(relayPin, LOW);
}
void printScreen()
{
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("DCBS: ");
  lcd.setCursor(10,0);
  lcd.print(state);
  lcd.setCursor(0,1);
  lcd.print("Pdl frc:");
  lcd.setCursor(9,1);
  lcd.print(data1);
  lcd.setCursor(0,2);
  lcd.print("RPM    :");
  lcd.setCursor(9,2);
  lcd.print(data2);
  lcd.setCursor(0,3);
  lcd.print("SLND   : ");
  lcd.setCursor(9,3);
  lcd.print(state);
}

void peddle()
{
  pressure = scale.getGram();
  Serial.print(pressure);
//  Serial.println(" g");
  delay(10);
  if(pressure<50)
    data1 = 0;
   else data1 = pressure;
}
void rpm()
{
  rev2 = 0;
  interrupts();
  delay(1000);
  noInterrupts();
  Serial.println(rev2/8);
  data2 = (rev2/8)*60;
}
void loop() {
  // put your main code here, to run repeatedly:
  printScreen();
  
  timedpeddle.check();
  timedrpm.check();
  if((rev2/8)*60 >= 100 || pressure>=1000)
  {
    digitalWrite(relayPin,HIGH);
    state = "ON";
  }
  if((rev2/8)*60 < 100 && pressure<1000)
  {
    digitalWrite(relayPin,LOW);
    state = "OFF";
  }
}
