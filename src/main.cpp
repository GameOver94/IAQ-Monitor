#define LED_BUILTIN 2

#include <network.h>  // handling of WiFi and NTP
#include <display.h>  // handling of the e-ink display
#include <sensor.h>  // handling of de BME680


unsigned long nextUpdate = 0;


void setup()
{
  // initalize serial comunication
  Serial.begin(115200);
  Serial.println();
  Serial.println("----- begin setup -----");
  delay(100);
  
  // connect WiFi
  connectWiFi();
  delay(200);

  // set NTP time
  setNTP();
  delay(200);

    // initalize Sensor
  initSensor();
  delay(200);

  // initalize display
  initDisplay();
  delay(200);

}

void loop()
{
  tm timeinfo;

  if (millis() > nextUpdate || nextUpdate - millis() > 3600000)
  {
    getLocalTime(&timeinfo, 3000);
    nextUpdate = millis() + (60 - timeinfo.tm_sec) *1000;
    Serial.println("\n-----------------------------------------------------");
    Serial.println(&timeinfo, "Datum: %d.%m.%y  Zeit: %H:%M:%S");


    Serial.println("\n----- Update Puffer -----");
    drawFrame();  // clear display and draws Frame in buffer
    drawClock(timeinfo);

    newData();  // chekst for new Sensor Data

    drawData(iaqSensor.temperature, iaqSensor.humidity, iaqSensor.pressure/100, iaqSensor.iaq, iaqSensor.staticIaq, iaqSensor.iaqAccuracy);
    Serial.println("\n Gas Resitance: " + String(iaqSensor.gasResistance));
    Serial.println("IAQ static: " + String(iaqSensor.staticIaq));
    Serial.println("IAQ: " + String(iaqSensor.iaq));
    Serial.println("IAQ accuracy: " + String(iaqSensor.iaqAccuracy));

    Serial.println("\n----- Update Display -----");
    display.display(false);  // full update
    Serial.println("----- Update Done -----\n");
  }

  delay(500);

}

