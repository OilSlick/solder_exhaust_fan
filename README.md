This project uses a Metro Mini from Adafruit to control a 16-LED Neopixel ring and a 5v fan, both to be used while soldering. 

The LED is controlled by a momentary switch. There are three levels of brightness that can be cycled through by repeatedly pressing the switch: first click = 33% brightness, second click = 66%, third click = 100%, fourth click = off. 

The fan is controlled by a separate momentary switch. 

The Metro Mini is running 3.3v logic. I use a Power Boost 1000C, also from Adafruit, to get 5v for the LED and fan, and also to charge the 18650 battery. 

The Metro Mini is overkill, of course. A microcontroller really isn't needed for this but it was fun to build and as one of my earliest projects, was a learning opportunity. The Metro Mini also consumes too much power at idle. An ATTiny would be a better solution. 

More detail: http://blog.michaelcwright.com/2017/03/28/diy-bench-light-and-solder-fan/
 
### To-Do
* [ ] Turning off LED still turns off fan. Troubleshoot.