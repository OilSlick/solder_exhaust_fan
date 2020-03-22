//Button code from https://www.arduino.cc/en/Tutorial/Debounce
//Metro Mini select "Uno" for board

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <Narcoleptic.h>

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
//#define NUMPIXELS 16                  // Popular NeoPixel ring size
//Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Following using simple_new_operator example
int pin = 1;
int numPixels = 16;                   // Popular NeoPixel ring size
int pixelFormat = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel *pixels;
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

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
bool RailOn = 0;                      //Tracks 5V rail

void setup() {  
  //Serial.begin(9600);
  //pinMode(lboPin,INPUT);
  //digitalWrite(lboPin,HIGH);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);             //turn off power boost (5v) 
  pinMode(FANSwitch,OUTPUT);           //Power to this pin activates PN2222 transistor switch (for fan)
  digitalWrite(FANSwitch,LOW);
  pinMode(FANButtonPin,INPUT);
  pinMode(LEDButtonPin, INPUT);

  //pinMode(FANLEDPin,INPUT);          //Pin to power LED in fan button
  pinMode(LiPoPin,INPUT);
  
  //For Neopixel
  pixels = new Adafruit_NeoPixel(numPixels, pin, pixelFormat);
  pixels->begin();                     // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels->clear();                     // Set all pixel colors to 'off'
  
  //Power management
  if ( !Serial )
  {
    power_usart0_disable();
  }
  power_spi_disable();
  power_twi_disable();
  /*if ( Serial ) 
  {
    float measuredvbat = analogRead(LiPoPin);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage
    Serial.print("vbat: ");
    Serial.println(measuredvbat);
    Serial.println("Ready");
  } */   
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
      if ( RailOn == 0 ) 
      {
        digitalWrite(enPin,HIGH);           //Power up 5v rail
        RailOn = 1;
      }
      digitalWrite(FANSwitch,HIGH);           //Turn on fan
      FANflag = 1;
      lastDebounceTime = millis();
    }
    else if ( (FANbuttonState == HIGH) && (FANflag == 1) ) {
      digitalWrite(FANSwitch,LOW);           //Turn off fan 
      FANflag = 0;
      lastDebounceTime = millis();
    } 
    
    //if the LED button has been pressed, toggle the Neopixel ring
     if ( (LEDbuttonState == HIGH) && (LEDflag == 0) ) 
     {
      if ( RailOn == 0 )
      {
        digitalWrite(enPin,HIGH);           //Power up 5v rail
        RailOn = 1;
      }
      
      WhiteLight(200);  //33 = 1/8, 63 = 1/4, 127 = 1/2, 255 = full
      LEDflag = 1;
      lastDebounceTime = millis();
    }
    else if ( (LEDbuttonState == HIGH) && (LEDflag == 1) ) {
      WhiteLight(0); //33 = 1/8, 63 = 1/4, 127 = 1/2, 255 = full 
      LEDflag = 0;
      lastDebounceTime = millis();
    }
    if ( LEDflag == 0 && FANflag == 0 )
    {
      digitalWrite(enPin,LOW);            //turn off power boost (5v) 
      RailOn = 0;
    }
    Narcoleptic.delay(100);
  }
}
/*void ActivateLED(int LEDPower)
{
  colorWipe(strip.Color(LEDPower, LEDPower, LEDPower), 50); 
}*/
void showBattLevel()
{
  //pixels.setPixelColor(4, pixels.Color(0, 150, 0));  //Green
  //pixels.show();   // Send the updated pixel colors to the hardware.
}
void WhiteLight(int LEDPower)
{
  for(int i=0; i<numPixels; i++) 
  { 
    // For each pixel...
    // pixels->Color() takes RGB values, from 0,0,0 up to 255,255,255
    pixels->setPixelColor(i, pixels->Color(LEDPower, LEDPower, LEDPower));
    pixels->show();   // Send the updated pixel colors to the hardware.
    delay(50); // Pause before next pass through loop
  }
}
