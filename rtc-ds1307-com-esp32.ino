// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include "time.h"
#include<WiFi.h>
#include <NTPClient.h>

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char *ssid     = "Erick";     //  WiFi name
const char *password = "******";    // WiFi password

int a = 1;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 0;

WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "in.pool.ntp.org", -18000, 60000);
char dayWeek [7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  Serial.begin(57600);
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("IP ad :");
  Serial.println(WiFi.localIP());
  delay(10);

  timeClient.begin();
  delay(10);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop () {
  if (a==1){ 
    timeClient.update();
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    String diaText = (&timeinfo, "%A");
    int mes =  timeinfo.tm_mon + 1;
    int dia =  timeinfo.tm_mday;
    int Year = timeinfo.tm_year +1900;
    int hora =  timeinfo.tm_hour;
    int minutos = timeinfo.tm_min;
    int segundos = timeinfo.tm_sec;
    delay(1000);
    rtc.adjust(DateTime(Year,mes,dia, hora, minutos, segundos));
  }  
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.println();
    delay(1000);
}
