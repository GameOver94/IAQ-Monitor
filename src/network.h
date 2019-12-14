#include <WiFi.h>
#include <time.h>
#include <credentials.h>

void connectWiFi();
void setNTP();

const char NTP_SERVER[] = "at.pool.ntp.org";
const char TZ_INFO[] = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";    // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
const char daysOfTheWeek[7][11] = {"Sonntag", "Mondtag", "Dienstag", "Mitwoch", "Donnerstag", "Freitag", "Samstag"};


void connectWiFi()
{
  Serial.println("----- connnect to WifFi -----");
  WiFi.begin(ssid, password);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    if (++counter > 100) ESP.restart();
    Serial.print ( "." );
  }
  Serial.println("----- WiFi connencted -----");
}

void setNTP()
{
  tm timeinfo;

  Serial.println("----- set NTP time -----");

  configTzTime(TZ_INFO, NTP_SERVER);
  getLocalTime(&timeinfo, 10000);
  Serial.println(&timeinfo, "Datum: %d.%m.%y  Zeit: %H:%M:%S");
}