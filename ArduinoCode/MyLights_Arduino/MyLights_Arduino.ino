/*
 * Circuit on Arduino for MyLights hardware setup:
 * Pin 6 - Data Pin of NeoPixel LED
 * Pin TX - PC_4 of Tiva
 * Pin RX - PC_5 of Tiva
 * Pin 9 - Red LED
 * Pin 10 - Green LED
 * 5v - Vcc of NeoPixel LED
 * GND - GND of NeoPixel LED
 * GND (second) - common GND of RED & GREEN LEDs
*/

#include <Adafruit_NeoPixel.h>

#define PIN            6
#define NUMPIXELS      5
#define RED_LED_PIN    9
#define GREEN_LED_PIN  10

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(7,0);
  Serial.begin(9600);
  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly: 

  char input[100] = "";
  if(Serial.available()){
    Serial.readBytesUntil('\n',input,20);
    Serial.println("Got request");
    Serial.println(input);

    String req = String(input);

    if (req.indexOf("/rgb/") != -1){
    
      Serial.print("The req is ");
      Serial.println(req);
    
      String bStr = req.substring(req.lastIndexOf(',') + 1);
    
      req.remove(req.lastIndexOf(','));
    
      String gStr = req.substring(req.lastIndexOf(',') + 1);
    
      req.remove(req.lastIndexOf(','));
    
      String rStr = req.substring(req.lastIndexOf('/') + 1);
    
      Serial.print("R = ");
      Serial.println(rStr.toInt());
    
      Serial.print("G = ");
      Serial.println(gStr.toInt());
    
      Serial.print("B = ");
      Serial.println(bStr.toInt());

      for(int pixel = 0; pixel <= pixels.numPixels(); pixel++){
        pixels.setPixelColor(pixel, pixels.Color(rStr.toInt(),gStr.toInt(),bStr.toInt()));
      }

      pixels.show(); // This sends the updated pixel 
    }
    else if(req.indexOf("/bulb/") != -1){
      Serial.print("The req is ");
      Serial.println(req);
    
      String bulbVal = req.substring(req.lastIndexOf('/') + 1);
      digitalWrite(7,bulbVal.toInt());
    }
    else if(req.indexOf("/led/green/") != -1){
      Serial.print("The req is ");
      Serial.println(req);
    
      String ledVal = req.substring(req.lastIndexOf('/') + 1);
      analogWrite(GREEN_LED_PIN,ledVal.toInt());
    }
    else if(req.indexOf("/led/red/") != -1){
      Serial.print("The req is ");
      Serial.println(req);
    
      String ledVal = req.substring(req.lastIndexOf('/') + 1);
      analogWrite(RED_LED_PIN,ledVal.toInt());
    }
  }
}
