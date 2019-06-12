/*
 * Project RemoteLabel
 * Description: A simple remote controllable e-paper label 
 * Author: dxcfl
 * Date: 21.03.2019
 */

// -----------------------------------------------
// Controlling an epaper display over the Internet
// -----------------------------------------------

/*
   IMPORTS & DEFINITIONS
 */

// Waveshare e-paper module
#include <Particle.h>
#include <Arduino.h>
#include <epd1in54.h>
#include <epdpaint.h>

// E-ink colours
#define COLORED 0
#define UNCOLORED 1

/*
   VARIABLES
 */

// controlling debugging ...
boolean debugging = true;

// E-paper image buffer and handle
unsigned char image[5000];
Paint paint(image, 200, 200);
Epd epd;

/*
   SETUP
 */
void setup()
{
  // Initialize e-paper
  debugMessage("initializing epaper ...");
  int epd_rc = epd.Init(lut_full_update);
  if (epd_rc != 0)
  {
    debugMessage("epaper intialization failed!");
    return;
  }

  debugMessage("registering functions ...");
  // We are going to declare a Particle.function so that we can turn the debugging on and off from the cloud.
  Particle.function("debug", debugToggle);
  // This is saying that when we ask the cloud for the function "debug", it will employ the function debugToggle() from this app.

  // We are also going to declare a Particle.function so that we can switch the sign from the cloud.
  Particle.function("show", showText);
  // This is saying that when we ask the cloud for the function "show", it will employ the function signToggle() from this app.

  debugMessage("ready.");
}

/*
   LOOP
 */
// Since we're waiting for input through the cloud this time,
// we don't actually need to put anything in the loop
void loop()
{
  // Nothing to do here
}

/*
  FUNCTIONS
*/

// Logging debug messages to cloud as DEBUG event ...

void debugMessage(String message)
{
  if (!debugging)
    return;
  Spark.publish("DEBUG", message);
}

void debugInt(String name, int value)
{
  if (!debugging)
    return;
  char msg[50];
  sprintf(msg, "%s = %d", name.c_str(), value);
  debugMessage(msg);
}

void debugStr(String name, String value)
{
  if (!debugging)
    return;
  char msg[256];
  sprintf(msg, "%s = ''%s'", name.c_str(), value.c_str());
  debugMessage(msg);
}

int debugToggle(String command)
{
  if (command == "on" or command == "true ")
  {
    debugging = true;
    return 1;
  }
  else if (command == "off" or command == "false ")
  {
    debugging = false;
    return 1;
  }
  else
  {
    return -1;
  }
}

// We're going to have a super cool function now that gets called when a matching API request is sent
// This is the showText function we registered to the "show" Particle.function earlier.
int showText(String command)
{
  /* Particle.functions always take a string as an argument and return an integer.
    The given string will be passed to the ePaperDisplayText function as parameter for
    the text to display.
    */

  debugMessage("command received ...");

  if (command == "CLEAR")
  {
    debugMessage("command: 'CLEAR'");
    ePaperClear();
    return 0;
  }
  else
  {
    debugMessage("command: text ...");
    ePaperDisplayText(true, 10, 10, 24, command);
    return 1;
  }
}


// E-paper functions ...

// Clear e-ink display:
void ePaperClear()
{
  /**
     From epd1in54-demo.ino - Waveshare 1.54inch e-paper display demo:
     There are 2 memory areas embedded in the e-paper display
     and once the display is refreshed, the memory area will be auto-toggled,
     i.e. the next action of SetFrameMemory will set the other memory area
     therefore you have to clear the frame memory twice.
  */

  debugMessage("clearing e-paper ...");

  epd.ClearFrameMemory(0xFF); // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF); // bit set = white, bit reset = black
  epd.DisplayFrame();

  debugMessage("e-paper cleared.");
}

// Display text on e-ink display:
void ePaperDisplayText(boolean clean, int posX, int posY, int fontSize, String text)
{
  debugMessage("showning text ...");
  debugStr("text",text);
  debugInt("font-size",fontSize);

  paint.SetRotate(ROTATE_180);

  if (clean)
  {
    paint.Clear(UNCOLORED);
  }

  sFONT *font;
  switch (fontSize)
  {
    case 8:
      font = &Font8;
      break;
    case 12:
      font = &Font12;
      break;
    case 16:
      font = &Font16;
      break;
    case 20:
      font = &Font20;
      break;
    case 24:
      font = &Font24;
      break;
    default:
      font = &Font24;
      break;
  }

  int i = 0;
  String line  = getToken(text,';',i);
  while ( line != "" ) {
    paint.DrawStringAt(posX, posY+(i*fontSize), line.c_str(), font, COLORED);
    line = getToken(text,';',++i);
  }
  debugInt("lines",1);

  epd.SetFrameMemory(image);
  epd.DisplayFrame();

  debugMessage("text showing.");
}


// Display image on e-ink display:
void ePaperShowImage(const unsigned char image_data[])
{
  /**
      From epd1in54-demo.ino - Waveshare 1.54inch e-paper display demo:
      There are 2 memory areas embedded in the e-paper display
      and once the display is refreshed, the memory area will be auto-toggled,
      i.e. the next action of SetFrameMemory will set the other memory area
      therefore you have to clear the frame memory twice.
  */

  debugMessage("showing image ...");

  epd.SetFrameMemory(image_data);
  epd.DisplayFrame();
  epd.SetFrameMemory(image_data);
  epd.DisplayFrame();

  debugMessage("image showing.");
}


// Aux functions ...

String getToken(String data, char separator, int index)
{
 int found = 0;
 int strIndex[] = {0, -1};
 int maxIndex = data.length()-1;

 for(int i=0; i<=maxIndex && found<=index; i++){
   if(data.charAt(i)==separator || i==maxIndex){
       found++;
       strIndex[0] = strIndex[1]+1;
       strIndex[1] = (i == maxIndex) ? i+1 : i;
   }
 }

 return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
