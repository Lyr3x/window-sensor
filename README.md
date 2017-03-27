# Cheap MySensors Window-/Door-Sensor
The code uses the awesome MySensors Framework to build a very cheap and reliable battery powered window or door sensor.
To monitor the battery level i added my own battery monitoring library, which is still under development and will be fully released soon.

 ## PlatformIO
 Im using the great PlatformIO-IDE (in Atom) for microcontroller development. This repo is ready to use with PlatformIO, if you use an Arduino Pro Mini 3,3V@8MHz (or just a Atmega328@8MHz). If you want to use the 16MHz version or the popular UNO, just uncomment the right board inside the platformio.ini

 ## Low Power Arduino
 To get a reasonable battery life with your Arduino Pro Mini, you will need to solder out the voltage regulator and the damn power-led. If you dont know how to do this, just ask google. There are a lot of good guides out there.

 # Credits
 Thanks to the MySensors Team to provide the best open source Smart Home DIY library!
 
