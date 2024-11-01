/* 
 * Project Midterm IoT
 * Author: Caprice Harwood
 * Date: 30-OCT-24
 */
//why wemos , why colors , why LEDS ? , how switch wemo (have 2 wemos)

//REMOVED STRING and SHORTHAND (shortcuts).


//Libaries used.
#include "Particle.h"
#include"IoTClassroom_CNM.h" //includes:Colors,Hue,button,wemo,timer.???
#include "Adafruit_BME280.h"
#include "Adafruit_SSD1306.h"
#include "Colors.h"
#include "Math.h"
#include "Hue.h"
#include "Button.h"

SYSTEM_MODE(MANUAL);

//
Button YButton(D5);
Adafruit_BME280 bme;
Adafruit_SSD1306 display(-1);


//Header declaration:
const int hexAddress = 0x76; //BME280 12C
const int MYWEMO = 4; //wemo 4 below 72
const int MYWEMO5 = 5;//above 72
const int YELLOWLED = D3;
const int BULB = 5; //hue
const int bri = 255;
//Variables 
bool status; //if staus
bool manualMode = false; //manual mode
float tempF; 
float temperature;
float temp;
bool toggled; //on/off refer to when we used toggle to make work

//intilaizing functions w BME 
void displayInfo(float temp, String hueColor); // float temp call upon a color (not sure if I can use color)




void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected,10000);

  //Wifi setup 
  WiFi.on();
  WiFi.clearCredentials();
  WiFi.setCredentials("IoTNetwork");
  WiFi.connect();
  Serial.println("Connected to Wifi");

//BME280 Setup
status = bme.begin(0x76);
if (!status){
  Serial.printf("BME280 Failed to start");
}

//OLED setup 
display.begin(SSD1306_SWITCHCAPVCC,0x3C); 
display.clearDisplay();


pinMode(YELLOWLED,OUTPUT);
}

//BME280 reads temp and converts to Faraenheight 
void loop() {
  digitalWrite(YELLOWLED,LOW);
  tempF = bme.readTemperature() * 1.8 + 32; //convert C to F
  if (YButton.isClicked()){
    manualMode = !manualMode; //Manual mode is off.
    Serial.printf("Manual mode toggled");
  }
  // Manual Mode LED indicator 
while (manualMode){
  digitalWrite(YELLOWLED,HIGH);
  if (YButton.isClicked()){
    manualMode = false; 
    Serial.printf("Manual mode toggled");
  }
} 
//auto
if(!manualMode){
//links hue and wemo to BME when in  manual is toggled

  if(tempF == 72){
    setHue(BULB,HueYellow,bri,255);
    wemoWrite(MYWEMO,LOW); //deafult
    wemoWrite(MYWEMO5,LOW);
    displayInfo(tempF,"Yellow");
  }
if(tempF > 72){
  setHue(BULB,HueRed,bri,255);
  wemoWrite(MYWEMO5,HIGH); //AC
  wemoWrite(MYWEMO,LOW);
}
if( tempF < 72 ){
  setHue(BULB,HueBlue,bri,255);
  wemoWrite(MYWEMO,HIGH); //heater
  wemoWrite(MYWEMO5,LOW);
}
delay(1000);


}

}
//OLED used function to tell OLED to display BME readings 
void displayInfo(float temp, String hueColor){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("Temperature %0.1f\nHue Color: %s\n",temp,hueColor.c_str());
  display.display();

}
