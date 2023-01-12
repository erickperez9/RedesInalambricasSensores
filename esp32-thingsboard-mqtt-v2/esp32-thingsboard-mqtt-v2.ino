#include <DHT_U.h>
#include <DHT.h>
#include<WiFi.h>
#include <PubSubClient.h>
#include "base64.hpp"
#include <base64.h>

/*#define MQTT_SERVER   "192.168.18.101"
#define MQTT_USERNAME "JtQEFiOxG8zYj75RY4RJ"*/
#define MQTT_SERVER   "demo.thingsboard.io"
#define MQTT_USERNAME "sY8nDvO83Z7Jj56nhGub"

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

/*const char* ssid ="Familia_Perez";
const char* password = "e#74-25_Pe$";*/

const char* ssid = "Mateo_Molina";
const char* password = "Thomas3117";



long timeStamp = 4;



typedef union{
  struct {
    uint16_t t;
    uint16_t h:7;
    uint32_t ts:9;
  };
  byte datos[6];  //linea para la recuperacion de bits 
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
}

void loop() {
  uint16_t hum = humedad();
  uint16_t temp = temperatura();
  String aux1 = "a";
  String aux2 = ".";
  String aux3 = ".";
  Serial.print("TEMPERATURA = ");
  Serial.print(temp);
  Serial.print("\tHUMEDAD = ");
  Serial.println(hum);
  trama valores;
  valores.ts=timeStamp;
  valores.t=temp;
  valores.h=hum;
  // *******************Codificacion*******************************
  
  String toEncode_trama = String(valores.datos,6);
  String toEncode_ts = String(valores.ts);
  String toEncode_temp = String(valores.t);
  String toEncode_hum = String(valores.h);
  String encoded_TS = base64::encode(toEncode_ts); //codificacion time stamps
  String encoded_temp = base64::encode(toEncode_temp); // codificacion temp
  String encoded_hum = base64::encode(toEncode_hum); //codificacion hum
  String encoded_trama = base64::encode(toEncode_trama); //codificacion trama completa

  int lt=encoded_temp.length();
  int lh=encoded_hum.length();
  String enc_t2=encoded_temp;
  String enc_h2 = encoded_hum;
  enc_t2.remove(lt-1,1);
  enc_h2.remove(lh-1,1);
  Serial.println(enc_t2);
  Serial.println(enc_h2);
  Serial.println("\n");
  
  delay(1000);
  Serial.println("\n DATA ENCODED");
  Serial.print("Base64 Time Stamp: ");Serial.println(encoded_TS);
  Serial.print("Base64 Temp: ");Serial.println(encoded_temp);
  Serial.print("Base64 Hum: ");Serial.println(encoded_hum);
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
    
    //String dataJS = "{\"TEMPERATURA\":" + enc_t2 + ",\"HUMEDAD\":" + enc_h2 + "}";
    //String dataJS = "{\"TRAMA\":" + encoded_trama + "}";
    String dataJS = "{\"TEMPERATURA\":" + encoded_trama + ",\"HUMEDAD\":" + encoded_trama + ",\"TS\":" + encoded_trama + "}";
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
  //Serial.println(F("Humidity: "));
  //Serial.println(h);
  //return round(hu);
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
  //Serial.println(F("Temperature: "));
  //Serial.println(t);
  //return round(te);
  return(int(te));
}
