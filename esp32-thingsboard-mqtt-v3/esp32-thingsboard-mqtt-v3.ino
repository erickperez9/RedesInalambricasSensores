#include <RTClib.h>
#include "time.h" 
#include <DHT_U.h>
#include <DHT.h>
#include<WiFi.h>
#include <PubSubClient.h>
#include "base64.hpp"
#include <base64.h>

/*#define MQTT_SERVER   "192.168.18.101"
#define MQTT_USERNAME "JtQEFiOxG8zYj75RY4RJ"*/
#define MQTT_SERVER   "demo.thingsboard.io"
#define MQTT_USERNAME "kXCOsUy6bgZqYnon2HPi"

/*#define MQTT_SERVER   "demo.thingsboard.io"
#define MQTT_USERNAME "u2maY03gjblM3TDgNlQV"*/

#define MQTT_PORT     1883
#define MQTT_PASSWORD ""         
#define MQTT_NAME     "mqtt-test"

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 13   

WiFiClient client;
PubSubClient mqtt(client);
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;


/*const char* ssid ="Familia_Perez";
const char* password = "e#74-25_Pe$";*/
/*
const char* ssid = "LAPTOP-AT5QP9T4";
const char* password = "383oA371";
*/
const char* ssid = "Mateo_Molina";
const char* password = "Thomas3117";

// NTP server to request epoch time
const char* ntpServer = "pool.ntp.org";

// Variable to save current epoch time
unsigned long epochTime; 

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

typedef union{
  struct {
    uint32_t ts;
    uint32_t h:9;
    uint32_t t:7;
  };
  byte datos[9];  //linea para la recuperacion de bits 
}trama;


void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(10);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("Connecting to WiFi..");
}
  Serial.println("IP ad :");
  Serial.println(WiFi.localIP());
  delay(10);
  
  mqtt.setServer(MQTT_SERVER, MQTT_PORT); 
  delay(10);
  configTime(0, 0, ntpServer);
}


void loop() {
  long timeStamp = 6;//getTime();
  Serial.println(timeStamp);
  uint32_t hum = humedad();
  uint32_t temp = temperatura();
  Serial.print("TEMPERATURA = ");
  Serial.print(temp);
  Serial.print("\tHUMEDAD = ");
  Serial.println(hum);
  trama valores;
  valores.ts=timeStamp;
  valores.t=temp;
  valores.h=hum;
  // *******************Codificacion*******************************
  
  String toEncode_trama = String(valores.datos,9);
  String encoded_trama = base64::encode(toEncode_trama); //codificacion trama completa

  
  delay(1000);
  Serial.println("\n DATA ENCODED");
  Serial.print("Base64 Trama comp: ");Serial.println(encoded_trama);
  
  if (mqtt.connected() == false) {
    Serial.print("MQTT connection... ");
    if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
    } else {
      Serial.println("failed");
      delay(5000);
    }
  } else {
    mqtt.loop();
    
    String dataJS = "{\"TRAMA\":" + encoded_trama + "}";
    char json[100];
    dataJS.toCharArray(json,dataJS.length()+1);
    mqtt.publish("v1/devices/me/telemetry", json);
    delay(1000);
  }
  
}


int humedad(){
  float hu = dht.readHumidity();
  float te = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(hu) || isnan(te) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  float hif = dht.computeHeatIndex(f, hu);
  float hic = dht.computeHeatIndex(te, hu, false);
  return(int(hu));
 }

int temperatura(){
  float hu = dht.readHumidity();
  float te = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(hu) || isnan(te) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  float hif = dht.computeHeatIndex(f, hu);
  float hic = dht.computeHeatIndex(te, hu, false);
  return(int(te));
}
