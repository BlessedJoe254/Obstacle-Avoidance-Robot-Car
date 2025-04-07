//led_control:
/*Inside the led controller file we have set up, declared, and defined the pins
 where the neopixel LEDs are connected. To control the neopixels we have used
  the Adafruit_NeoPixel library.*/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define F_LED_PIN        12 //Front LED
#define F_LED_PIXEL       6
#define B_LED_PIN        13  //Back LED
#define B_LED_PIXEL       6
#define R_LED_PIN        7 //Right LED
#define R_LED_PIXEL       1
#define L_LED_PIN        8 //Left LED
#define L_LED_PIXEL       1
Adafruit_NeoPixel F_LED_STRIP(F_LED_PIXEL, F_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel B_LED_STRIP(B_LED_PIXEL, B_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel R_LED_STRIP(R_LED_PIXEL, R_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel L_LED_STRIP(L_LED_PIXEL, L_LED_PIN, NEO_GRB + NEO_KHZ800);

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
  

  /*Inside this function, I have initialized all the neopixels LEDs and cleared
   them out with the default function provided by Adafruit, so that the neopixels
    don't light up randomly at the start of the code.*/
void LED_setup()
{
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  F_LED_STRIP.begin();
  B_LED_STRIP.begin();
  R_LED_STRIP.begin();
  L_LED_STRIP.begin();
  R_LED_STRIP.clear();
  L_LED_STRIP.clear();
  F_LED_STRIP.clear();
  B_LED_STRIP.clear();
}

void R_LED(int a)
{
  if(a==1)
  {
    R_LED_STRIP.clear();
    R_LED_STRIP.setPixelColor(0, R_LED_STRIP.Color(0, 255, 255));
   R_LED_STRIP.show();   // Send the updated pixel colors to the hardware.
  }
  else {
    R_LED_STRIP.clear();
     R_LED_STRIP.show();
  }   
}
void L_LED(int a)
{
  if(a==1)
  {
    L_LED_STRIP.clear();
    L_LED_STRIP.setPixelColor(0, L_LED_STRIP.Color(0, 255, 255));
   L_LED_STRIP.show();   // Send the updated pixel colors to the hardware.
  }
  else
  {
     L_LED_STRIP.clear();
     L_LED_STRIP.show();
  }
}
void F_LED(int a)
{
  if(a==1)
  {
    F_LED_STRIP.clear();
    F_LED_STRIP.setPixelColor(0, F_LED_STRIP.Color(255, 255, 255));
    F_LED_STRIP.setPixelColor(1, F_LED_STRIP.Color(0, 0, 50));
    F_LED_STRIP.setPixelColor(2, F_LED_STRIP.Color(0, 0, 50));
    F_LED_STRIP.setPixelColor(3, F_LED_STRIP.Color(0, 0, 50));
    F_LED_STRIP.setPixelColor(4, F_LED_STRIP.Color(0, 0, 50));
    F_LED_STRIP.setPixelColor(5, F_LED_STRIP.Color(255, 255, 255));
   F_LED_STRIP.show();   // Send the updated pixel colors to the hardware.
  }
  else if(a==0)
  {
    F_LED_STRIP.clear();
    F_LED_STRIP.show();
  }
}

// functions that control all the LEDs
void B_LED(int a)
{
  if(a==1)
  {
    B_LED_STRIP.clear();
    B_LED_STRIP.setPixelColor(0, B_LED_STRIP.Color(255, 0, 0));
    B_LED_STRIP.setPixelColor(1, B_LED_STRIP.Color(0, 50, 0));
    B_LED_STRIP.setPixelColor(2, B_LED_STRIP.Color(0, 50, 0));
    B_LED_STRIP.setPixelColor(3, B_LED_STRIP.Color(0, 50, 0));
    B_LED_STRIP.setPixelColor(4, B_LED_STRIP.Color(0, 50, 0));
    B_LED_STRIP.setPixelColor(5, B_LED_STRIP.Color(255, 0, 0));
    B_LED_STRIP.show();   // Send the updated pixel colors to the hardware.
  }
  else if(a==0)
  {
    B_LED_STRIP.clear();
    B_LED_STRIP.show();
  }
}


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  mySerial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Sending AT...");
  mySerial.write("AT");
  if (mySerial.available()>0) {
    Serial.write(mySerial.read());
  }

  // set the data rate for the SoftwareSerial port
    Serial.println("loop begins");
     LED_setup();

}
void loop() { // run over and over
  if (mySerial.available()) {
    rcd = mySerial.read();
    Serial.write(rcd);
  }
  unsigned long time_now = 0;
  if (rcd == 'O')
  {
    R_LED(1);
    L_LED(1);
    F_LED(1);
    B_LED(1);
  }
  if (rcd == 'o')
  {
    R_LED(0);
    L_LED(0);
    F_LED(0);
    B_LED(0);
  }
  if (rcd == 'X')
  { L_LED(1);
  }
  if (rcd == 'x')
  {
    L_LED(0);
  }
  if (rcd == 'P')
  {
    R_LED(1);
  }
  if (rcd == 'p')
  { R_LED(0);
  }
  if (rcd == 'W')
  { F_LED(1);
  }
  if (rcd == 'w')
  { F_LED(0);
  }
  if (rcd == 'U')
  { B_LED(1);
  }
  if (rcd == 'u')
  { B_LED(0);
  }
  if (rcd == '0')
  { spd = 80;
  }
  if (rcd == '1')
  { spd = 100;
  }
  if (rcd == '3')
  { spd = 120;
  }
  if (rcd == '5')
  { spd = 180;
  }
  if (rcd == 'q')
  { spd = 255;
  }
  if (rcd == 'F')
  { Forward();
    time_now = millis();
    while (millis() < time_now + 100) {}
  }
  else if (rcd == 'B')
  { time_now = millis();
    while (millis() < time_now + 100) {}
    Backward();
  }
  else if (rcd == 'I')
  { time_now = millis();
    while (millis() < time_now + 100) {}
    FdRight();
  }
  else if (rcd == 'J')
  { time_now = millis();
    while (millis() < time_now + 100) {}
    BkRight();
  }
  else if (rcd == 'R')
  { time_now = millis();
    while (millis() < time_now + 100) {}
    SharpRight();
  }
  else if (rcd == 'L')
  { time_now = millis();
    while (millis() < time_now + 100) {}
   SharpLeft();
  }
  else if (rcd == 'G')
  { time_now = millis();
    while (millis() < time_now + 100) {}
   FdLeft();
  }
  else if (rcd == 'H')
  { time_now = millis();
    while (millis() < time_now + 100) {}
    BkLeft();
  }
  else
    Stop();
}

//Movements :

void Forward()
{
  analogWrite(5, spd);
  digitalWrite(6, 0);
  analogWrite(9, spd);
  digitalWrite(3, 0);
  //  delay(50);
}
void Backward()
{
  digitalWrite(5, 0);
  analogWrite(6, spd);
  digitalWrite(9, 0);
  analogWrite(3, spd);
}
void Stop()
{
  analogWrite(5, 0);
  digitalWrite(6, 0);
  analogWrite(9, 0);
  digitalWrite(3, 0);
}
void FdRight()
{
  analogWrite(5, spd);
  digitalWrite(6, 0);
  digitalWrite(9, 0);
  analogWrite(3, 0);
}
void BkRight()
{
  analogWrite(5, 0);
  digitalWrite(6, 0);
  digitalWrite(9, 0);
  analogWrite(3, spd);
}
void SharpRight()
{
  analogWrite(5, spd);
  digitalWrite(6, 0);
  digitalWrite(9, 0);
  analogWrite(3, spd);
}
void SharpLeft()
{
  digitalWrite(5, 0);
  analogWrite(6, spd);
  analogWrite(9, spd);
  digitalWrite(3, 0);
}
void BkLeft()
{
  digitalWrite(5, 0);
  analogWrite(6, spd);
  analogWrite(9, 0);
  digitalWrite(3, 0);
}
void FdLeft()
{
  digitalWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, spd);
  digitalWrite(3, 0);
}


//link to the controller application
//https://circuitdigest.com/sites/default/files/other/BLE_Controller.zip




