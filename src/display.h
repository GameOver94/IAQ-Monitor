// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V


// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>

// fonts
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

// for converting float to string with fixed number of digits
#include <StreamString.h>
#define PrintString StreamString

GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=5*/ 5, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));



const char HelloWorld[] = "Hello World!";



void intDisplay();
void helloWorld();
void drawFrame();
void drawClock(tm timeinfo);
void drawData(float temp, float hum, float red_pres, float IAQ, float IAQ_static, uint8_t IAQ_accuracy);



void initDisplay() // initiatize display and draw sartup screen
{
  display.init(115200);
  // first update should be full refresh
  helloWorld();
}


void helloWorld()
{
  Serial.println("----- helloWorld -----");

  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  // full window mode is the initial mode, set it anyway
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.setCursor(x, y);
  display.print(HelloWorld);
  display.display(false); // full update

  Serial.println("----- helloWorld done -----");
}


void drawFrame()
{
  Serial.println("drawFrame");

  display.setRotation(0);
  display.setFullWindow();

  display.fillScreen(GxEPD_BLACK);
  display.fillRect(2, 2, 124, 108, GxEPD_WHITE);
  display.fillRect(2, 112, 124, 38, GxEPD_WHITE);
  display.fillRect(2, 152, 124, 38, GxEPD_WHITE);
  display.fillRect(2, 192, 124, 38, GxEPD_WHITE);
  display.fillRect(2, 232, 124, 62, GxEPD_WHITE);

}


void drawClock(tm timeinfo)
{
  Serial.println("drawTime");

  PrintString valueString;  // String buffer for type conversion

  // ----------- Time -----------
  display.setFont(&FreeMonoBold18pt7b);
  display.setTextColor(GxEPD_BLACK);

  valueString.print(&timeinfo, "%H:%M");

  // calcuate Text position
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(valueString, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = 20 + tbh;

  display.setCursor(x, y);
  display.print(valueString);


  // ----------- Date -----------
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  // calcuate Text position
  display.getTextBounds(daysOfTheWeek[timeinfo.tm_wday], 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  x = ((display.width() - tbw) / 2) - tbx;
  y = 58 + tbh;

  display.setCursor(x, y);
  display.print(daysOfTheWeek[timeinfo.tm_wday]);

  valueString.clear();
  valueString.print(&timeinfo, "%d.%m");

    // calcuate Text position
  display.getTextBounds(valueString, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  x = ((display.width() - tbw) / 2) - tbx;
  y = 80 + tbh;

  display.setCursor(x, y);
  display.print(valueString);

}


void drawData(float temp, float hum, float red_pres, float IAQ, float IAQ_static, uint8_t IAQ_accuracy)
{
  PrintString valueString;  // String buffer for type conversion

  Serial.println("drawData");

  // Temperatrue
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);

  // converte float to string with fixed number of digits
  valueString.print(temp, 2);
  valueString.concat(" C");

  // calcuate Text position
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(valueString, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = 123 + tbh;

  display.setCursor(x, y);
  display.print(valueString);

  // centigrade symbol
  display.fillCircle(x + tbw - 16, 123 + 5, 4, GxEPD_BLACK);
  display.fillCircle(x + tbw - 16, 123 + 5, 2, GxEPD_WHITE);


   // Humidity

  // converte float to string with fixed number of digits
  valueString.clear();
  valueString.print(hum, 2);
  valueString.concat("%");

  // calcuate Text position
  display.getTextBounds(valueString, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  x = ((display.width() - tbw) / 2) - tbx;
  y = 163 + tbh;

  display.setCursor(x, y);
  display.print(valueString);


  // Pressure

  display.setFont(&FreeMonoBold9pt7b);

  // converte float to string with fixed number of digits
  valueString.clear();
  valueString.print(red_pres, 2);
  valueString.concat("hPa");

  // calcuate Text position
  display.getTextBounds(valueString, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  x = ((display.width() - tbw) / 2) - tbx;
  y = 204 + tbh;

  display.setCursor(x, y);
  display.print(valueString);


  // IAQ

  display.setFont(&FreeMonoBold24pt7b);

  // converte float to string with fixed number of digits
  valueString.clear();
  if (IAQ_accuracy == 1)
  {
    valueString.print(IAQ, 0);

    // locked symbol
    display.fillRect(112, 242, 8, 6, GxEPD_BLACK);
    display.fillRect(113, 238, 6, 4, GxEPD_BLACK);
    display.fillRect(114, 239, 4, 3, GxEPD_WHITE);

  } else
  {
    valueString.print(IAQ_static, 0);

    // unlocked symbol
    display.fillRect(112, 242, 8, 6, GxEPD_BLACK);
    display.fillRect(113, 237, 6, 5, GxEPD_BLACK);
    display.fillRect(114, 238, 4, 4, GxEPD_WHITE);
    display.drawPixel(118, 241, GxEPD_WHITE);

  }


  // calcuate Text position
  display.getTextBounds(valueString, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  x = ((display.width() - tbw) / 2) - tbx;
  y = 246 + tbh;


  display.setCursor(x, y);
  display.print(valueString);

}