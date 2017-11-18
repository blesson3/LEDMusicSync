#include "FastLED.h"

FASTLED_USING_NAMESPACE

// The LEDMusicSync project on arduino aims to implement a protocol
// through serial input for controlling individually addressable 
// LED strips.

// Matt Blessed -- 2017-11-15


#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    5
#define LED_TYPE    SK6812
#define COLOR_ORDER GRB
#define NUM_LEDS    100
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          20
#define FRAMES_PER_SECOND  120

// Protocol Buffer
char dataBuffer[200]; // Allocate some space for the string
byte inBufferIndex = 0;
int currentFadeDelta = 50;

// prototypes
void clearDataBuffer();
void processLEDData(char data[]);


void setup() {
  Serial.begin(115200);
  
  delay(3000); // 3 second delay for power recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  Serial.write("1"); // start command
}

void loop()
{  
  if (Serial.available()) {

    char c = Serial.read();
    dataBuffer[inBufferIndex] = c;
    inBufferIndex++;

    // look for the end of command signal
    if (c == '&') {
      processLEDData(dataBuffer); 
      clearDataBuffer();
    }
  }

  // send the 'leds' array out to the actual LED strip  
  // insert a delay to keep the framerate modest
  EVERY_N_MILLISECONDS(1000/FRAMES_PER_SECOND) {
    // always fade the LEDs in every loop
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].fadeToBlackBy(currentFadeDelta); // 50  currentFadeDelta
    }
    
    FastLED.show();
  }
}

void clearDataBuffer() {
  // reset flags
  inBufferIndex = 0;
  // clear array
  for( int i = 0; i < sizeof(dataBuffer);  ++i )
   dataBuffer[i] = (char)0;

  // add a null terminator at the end
  dataBuffer[sizeof(dataBuffer)-1] = '\0';
}

void processLEDData(char data[]) {

  String members[13];
  split(data, '|', members);

//  Serial.println("Members:");
//  for (int i = 0; i < 13; i++) {
//    Serial.println(members[i]);
//  }
//  Serial.println("printed all members.");
//  Serial.println("\n");

  // get indexes of leds we will be messing with
  int ledIndexes[NUM_LEDS];
  int ledIndexesIdx = 0;
  int numSections = members[0].toInt();
  for (int i = 0; i < numSections; i++) {
    String comps[2];
    split(members[i+1], ',', comps);
    
    int startIndex = comps[0].toInt();
    int len = comps[1].toInt();
    for (int j = startIndex; j <= startIndex+len; j++) {
      ledIndexes[ledIndexesIdx++] = j;
    }
  }

  if (ledIndexesIdx == 0 || numSections == 0) {
    Serial.write("0"); // print error
    return;
  }

  // get the color
  char colorChar = members[numSections+1].charAt(0);
  CRGB color;
  if (colorChar == 'r') {
    color = CRGB::Red;
  }
  else if (colorChar == 'g') {
    color = CRGB::Green;
  }
  else if (colorChar == 'b') {
    color = CRGB::Blue;
  }
  else if (colorChar == 'w') {
    color = CRGB::White;
  }
  else {
    Serial.write("0"); // print error
    return;
  }

  // get the fade delta
//  String fadeDeltaString = members[numSections+2];
//  fadeDeltaString.remove(fadeDeltaString.length()-1);
  int fadeDelta = members[numSections+2].toInt();
  if (fadeDelta == 0) {
    Serial.write("0"); // print error
    return;
  }
  currentFadeDelta = fadeDelta;

//  Serial.write("\nSections:");
//  Serial.write(numSections);
//  Serial.write(members[0].charAt(0));
//  Serial.write("\nColor:");
//  Serial.write(colorChar);
//  Serial.write("\nFade Delta:");
//  Serial.write(fadeDelta);
  

  for (int i = 0; i < ledIndexesIdx-1; i++) {
    int idx = ledIndexes[i];
    leds[idx] = color;
  }

  Serial.write("1");

  // first char is color (simple binary representation for RGB)
  // 0 = 000
  // 1 = 001
  // ...
  // 6 = 110
//  char alphaBuffer[2] = { data[1], data[2] };
//  int alpha = (int)strtol(alphaBuffer, NULL, 16);
//  
//  int colorChar = data[0] - '0';
//  int r = (colorChar & 4) == 4 ? alpha : 0;
//  int g = (colorChar & 2) == 2 ? alpha : 0;
//  int b = (colorChar & 1) == 1 ? alpha : 0;
//  CRGB color = CRGB(r, g, b);
//  for (int i = 0; i < NUM_LEDS; i++) {
//     leds[i] = color;
//  }
//  FastLED.show();

//  String members[NUM_LEDS]; // currently we have MAX two pieces of data (index and color, or just color)
//  String comps[3];
//  // split between the led index and the color information
//  split(data, ',', members);
//  
////  Serial.println(members[0]);
//  // if only one piece of information exists, it must be signaling to make all LEDs that color
//  if (members[1] == "\0") {
//    split(String(members[0]), '.', comps);
//    CRGB color = CRGB( comps[0].toInt(), comps[1].toInt(), comps[2].toInt());
//    for (int i = 0; i < NUM_LEDS; i++) {
//      leds[i] = color;
//    }
//    FastLED.show();
//  }
//  // else has two or more pieces of information
//  else {
//    // validate index
//    int ledIndex = members[0].toInt();
//    if (ledIndex >= NUM_LEDS) {
////      Serial.println("Error: index above number of possible LEDs");
////      Serial.println("Recovering...");
//    }
//    else {
//      // members [0] = index of led
//      //         [1] = color information
//    
//      split(String(members[1]), '.', comps);
//      leds[ledIndex] = CRGB( comps[0].toInt(), comps[1].toInt(), comps[2].toInt());
//      FastLED.show();
//    }
//  }
}

// first star is a pointer to a char, then the second star says it's a char array
void split(String str, char delimiter, String *dest) {
  int commaIndex = -1;
  int lastCommaIndex = -1;
  int i = 0;
  while (commaIndex = str.indexOf(delimiter, commaIndex+1)) {
    String sub = str.substring(lastCommaIndex+1, commaIndex);
    dest[i] = sub;
    
    // check if that was the last index
    if (commaIndex <= lastCommaIndex) {
      break;
    }
    
    i++;
    lastCommaIndex = commaIndex;
  }

  dest[++i] = "\0";
}

//
//#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
//
//void nextPattern()
//{
//  // add one to the current pattern number, and wrap around at the end
//  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
//}
//
//void rainbow() 
//{
//  // FastLED's built-in rainbow generator
//  fill_rainbow( leds, NUM_LEDS, gHue, 7);
//}
//
//void rainbowWithGlitter() 
//{
//  // built-in FastLED rainbow, plus some random sparkly glitter
//  rainbow();
//  addGlitter(80);
//}
//
//void addGlitter( fract8 chanceOfGlitter) 
//{
//  if( random8() < chanceOfGlitter) {
//    leds[ random16(NUM_LEDS) ] += CRGB::White;
//  }
//}
//
//void confetti() 
//{
//  // random colored speckles that blink in and fade smoothly
//  fadeToBlackBy( leds, NUM_LEDS, 10);
//  int pos = random16(NUM_LEDS);
//  leds[pos] += CHSV( gHue + random8(64), 200, 255);
//}
//
//void sinelon()
//{
//  // a colored dot sweeping back and forth, with fading trails
//  fadeToBlackBy( leds, NUM_LEDS, 20);
//  int pos = beatsin16(13,0,NUM_LEDS);
//  leds[pos] += CHSV( gHue, 255, 192);
//}
//
//void bpm()
//{
//  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
//  uint8_t BeatsPerMinute = 62;
//  CRGBPalette16 palette = PartyColors_p;
//  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
//  for( int i = 0; i < NUM_LEDS; i++) { //9948
//    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
//  }
//}
//
//void juggle() {
//  // eight colored dots, weaving in and out of sync with each other
//  fadeToBlackBy( leds, NUM_LEDS, 20);
//  byte dothue = 0;
//  for( int i = 0; i < 8; i++) {
//    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
//    dothue += 32;
//  }
//}

