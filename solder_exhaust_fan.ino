//Button code from https://www.arduino.cc/en/Tutorial/Debounce
//Metro Mini select "Uno" for board

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <Narcoleptic.h>

#define PIN 1
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

const int OnBoardLED = 13;            //Pin to control onboard LED
const int FANSwitch = 12;             //Base pin of PN2222 transistor (fan switch)
const int FANButtonPin = 11;          //Pushbutton pin
const int LEDButtonPin = 10;          //Pushbutton pin
const int enPin = 9;                  //Power Boost EN pin
const int lboPin = 8;                 //Power Boost LBO pin
const int FANLEDPin = A5;             //Pin to power LED in fan button
const int LiPoPin = A4;               //Pin connected to "LiPo" pin on Power Boost (to monitor bat level)

int LEDPower = 0;                     //33 = 1/8, 63 = 1/4, 127 = 1/2, 255 = full
int LEDbuttonState;                   //current reading from the input pin
int FANbuttonState;                   //current reading from the input pin
unsigned long lastDebounceTime = 0;   //last time the output pin was toggled
unsigned long debounceDelay = 125;    //debounce time; increase if the output flickers
long longPressTime = 250;             //Enable button long-press 
int LEDflag = 0;                      //tracks LEDbutton clicks
int FANflag = 0;                      //tracks FANbutton clicks

void setup() {  
  //Serial.begin(9600);
  pinMode(lboPin,INPUT);
  digitalWrite(lboPin,HIGH);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);            //turn off power boost (5v) 
  pinMode(FANSwitch,OUTPUT);          //Power to this pin activates PN2222 transistor switch (for fan)
  digitalWrite(FANSwitch,LOW);

  //pinMode(FANLEDPin,INPUT);          //Pin to power LED in fan button
  //pinMode(LiPoPin,INPUT);
  
  //For Neopixel
  strip.begin();
  strip.show();                         //All pixels off

  //Power management
  if ( !Serial )
  {
    power_usart0_disable();
  }
  power_spi_disable();
  power_twi_disable();
  if ( Serial ) {
    Serial.println("Ready");
  }

}

void loop() {
  /*if ( digitalRead(lboPin) == LOW ) 
  {
    digitalWrite(enPin,LOW);           //Power down Neopixel ring
    BlinkLED();                        //Signal impending doom
  }*/
  
  /*int rawReading = analogRead(LiPoPin);
  Serial.print("rawReading: "); Serial.println(rawReading);
  float volts = rawReading / 204.6;
  Serial.print("Volts: "); Serial.println(volts);
  */
 
  //sample the state of the button - is it pressed or not?
  LEDbuttonState = digitalRead(LEDButtonPin);
  FANbuttonState = digitalRead(FANButtonPin);        
 
  //filter out any noise by setting a time buffer
  if ( (millis() - lastDebounceTime) > debounceDelay) {

  //if the FAN button has been pressed, turn on fan
     if ( (FANbuttonState == HIGH) && (FANflag == 0) ) {
      digitalWrite(enPin,HIGH);           //Power up 5v rail
      digitalWrite(FANSwitch,HIGH);           //Turn on fan
      FANflag = 1;
      lastDebounceTime = millis();
    }
    else if ( (FANbuttonState == HIGH) && (FANflag == 1) ) {
      digitalWrite(FANSwitch,LOW);           //Turn off fan 
      digitalWrite(enPin,LOW);           //Power off 5v rail
      FANflag = 0;
      lastDebounceTime = millis();
    } 
    
    //if the LED button has been pressed, toggle the Neopixel ring
     if ( (LEDbuttonState == HIGH) && (LEDflag == 0) ) {
      digitalWrite(enPin,HIGH);           //Power up 5v rail
    
      ActivateLED(33); //33 = 1/8, 63 = 1/4, 127 = 1/2, 255 = full
      LEDflag = 1;
      lastDebounceTime = millis();
    }

    /*  Disable higher LED power levels—shit was annoying
    else if ( (LEDbuttonState == HIGH) && (LEDflag == 1) ) {
 
      ActivateLED(127); //33 = 1/8, 63 = 1/4, 127 = 1/2, 255 = full
      LEDflag = 2;
      lastDebounceTime = millis();
    }
    else if ( (LEDbuttonState == HIGH) && (LEDflag == 2) ) {
 
      ActivateLED(255); //33 = 1/8, 63 = 1/4, 127 = 1/2, 255 = full
      LEDflag = 3;
      lastDebounceTime = millis();
    }*/
    else if ( (LEDbuttonState == HIGH) && (LEDflag == 1) ) {
 
      ActivateLED(0); //33 = 1/8, 63 = 1/4, 127 = 1/2, 255 = full 
      LEDflag = 0;
      lastDebounceTime = millis();
    }
    if ( LEDflag == 0 && FANflag == 0 )
    {
      digitalWrite(enPin,LOW);            //turn off power boost (5v) 
    }
    Narcoleptic.delay(100);
  }
}
void BlinkLED()
{
  digitalWrite(OnBoardLED, HIGH);   
  delay(100);                       
  digitalWrite(OnBoardLED, LOW);    
  delay(100);                       
  digitalWrite(OnBoardLED, HIGH);   
  delay(100);                       
  digitalWrite(OnBoardLED, LOW);   
  delay(100);                       
  digitalWrite(OnBoardLED, HIGH);   
  delay(100);                       
  digitalWrite(OnBoardLED, LOW);    
  delay(100);                       
  digitalWrite(OnBoardLED, HIGH);  
  delay(100);                      
  digitalWrite(OnBoardLED, LOW);   
  delay(300);
}
void ActivateLED(int LEDPower)
{
  colorWipe(strip.Color(LEDPower, LEDPower, LEDPower), 50); 
}

