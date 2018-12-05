#include <FastLED.h>
#include <String.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


#define LED_PIN     13
#define NUM_LEDS    50
#define BRIGHTNESS  120
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

String response1 = "";
String response2 = "";
int incomingByte;
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);
extern uint8_t packetbuffer[];
int rgb = 0;
int code = 0;
int selected = 0;
uint8_t red=0;
uint8_t green=0;
uint8_t blue=0;
uint8_t buttnum = 0; 


void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    Serial.begin(115200);
    if ( !ble.begin(VERBOSE_MODE) )
    {
    Serial.println(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
    }
    Serial.print(F("Initializing: "));

    ble.echo(false);
    ble.verbose(false);
    while(!ble.isConnected())
    {
      delay(500);
    }
    ble.setMode(BLUEFRUIT_MODE_DATA);
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    blackLight();
    selector("");
}


void loop()
{
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);

  
  if(len==0) return;

  if(rgb == 1 && code ==0)
    Serial.println("Please use the color picker to select a color.");
   

  while(rgb == 1 && code ==0 && selected == 0){
    if (packetbuffer[1] == 'C') {
      red = packetbuffer[2];
      green = packetbuffer[3];
      blue = packetbuffer[4];
      Serial.print ("RGB #");
      if (red < 0x10) Serial.print("0");
      Serial.print(red, HEX);
      if (green < 0x10) Serial.print("0");
      Serial.print(green, HEX);
      if (blue < 0x10) Serial.print("0");
      Serial.println(blue, HEX);
      selected = 1;
    }
  }

  while(rgb == 0 && code == 1 && selected ==0){
    if (packetbuffer[1] == 'B') {
      buttnum = packetbuffer[2] - '0';
      boolean pressed = packetbuffer[3] - '0';
      Serial.print ("Button "); Serial.print(buttnum);
      if (pressed) {
        Serial.println(" pressed");
      } else {
        Serial.println(" released");
      }
    }
    selected = 1;
  }
 
   while(Serial.available()==0)
   {
      if(rgb)
      {
        setLight(red, green, blue);
        Serial.println(red);
        delay(100);
      }
      if(code)
      {
        switch(buttnum)
        {
          case 1: 
            christmasLight();
            break;
          case 2: 
            ambulanceLight();
            break;
          case 3:
            movingLED();
            break;
        }
      }
   }
   response2 = Serial.readString();
   Serial.println("You typed: " + response2); 

   if(response2.equalsIgnoreCase("q") || response2.equalsIgnoreCase("Stop") || response2.equalsIgnoreCase("quit"))
   {
    selector("");
    buttnum = 0;
   }
}

void setLight(int red, int green, int blue)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
          leds[i].setRGB(red, green, blue);
    }
   FastLED.show();
   delay(100);
}

void blackLight()
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB::Black;
          FastLED.show();
    }

}

void christmasLight()
{
    uint8_t brightness = 255;

    for(int i = 0; i < NUM_LEDS; i++)
    {
        if(i%2==0)
        {
          leds[i] = CRGB::Red;
        }
        else
        {
          leds[i] = CRGB::Green;
        }
    }
    FastLED.show();
    delay(1000);
    for(int j = 0; j < NUM_LEDS; j++)
    {
        if(j%2==0)
        {
          leds[j] = CRGB::Green;
        }
        else
        {
          leds[j] = CRGB::Red;
        }
    }
    FastLED.show();
    delay(1000);
}

void ambulanceLight()
{
    uint8_t brightness = 255;

    for(int i = 0; i < NUM_LEDS; i++)
    {
        if(i%2==0)
        {
          leds[i] = CRGB::Red;
        }
        else
        {
          leds[i] = CRGB::Blue;
        }
    }
    FastLED.show();
    delay(1000);
    for(int j = 0; j < NUM_LEDS; j++)
    {
        if(j%2==0)
        {
          leds[j] = CRGB::Blue;
        }
        else
        {
          leds[j] = CRGB::Red;
        }
    }
    FastLED.show();
    delay(1000);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB::Red;
          FastLED.show();
          delay(100);
          leds[i] = CRGB::Black;
        
        
        //leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        //colorIndex += 3;
    }
    for (int j = 49; j>=0; j--)
    {
      leds[j] = CRGB::Green;
      FastLED.show();
      delay(100);
      leds[j] = CRGB::Black;
    }
    delay(100);
}


void movingLED()
{
   for( int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB::Red;
          FastLED.show();
          delay(100);
          leds[i] = CRGB::Black;
        
        
        //leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        //colorIndex += 3;
    }
    for (int j = 49; j>=0; j--)
    {
      leds[j] = CRGB::Green;
      FastLED.show();
      delay(100);
      leds[j] = CRGB::Black;
    }
    delay(100);
}

void selector(String input)
{
  blackLight();
  rgb = 0; code = 0; selected = 0;
  Serial.println("Code or RGB");
  while(Serial.available()==0){}
  response1 = Serial.readString();
  Serial.println(response1);
  if(response1.equalsIgnoreCase("rgb"))
  {
    rgb = 1;
  }
  else if (response1.equalsIgnoreCase("code"))
  {
    code =1;
  }
  else
  {
    Serial.println("Not a valid response. Please try again!");
    selector("");
  }
  Serial.println("Moving On!");
  Serial.println(code);
  
}



// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
