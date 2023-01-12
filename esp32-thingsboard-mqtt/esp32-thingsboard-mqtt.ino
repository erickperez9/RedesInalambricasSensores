#include <DHT_U.h>
#include <DHT.h>
#include<WiFi.h>
#include <PubSubClient.h>

#define MQTT_SERVER   "192.168.43.26"
#define MQTT_USERNAME "n8FmbefZpG15X2zzHKfr"

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

const char* ssid = "Erick";
const char* password = "tnbt8311+";

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
  float temp = temperatura();
  float hum = humedad();
  Serial.print("TEMPERATURA = ");
  Serial.print(temp);
  Serial.print("\tHUMEDAD = ");
  Serial.println(hum);
    
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
    String dataJS = "{\"TEMPERATURA\":" + String(temp,3) + ",\"HUMEDAD\":" +String(hum, 3) + "}";
    char json[100];
    dataJS.toCharArray(json,dataJS.length()+1);
    mqtt.publish("v1/devices/me/telemetry", json);
    delay(1000);
  }
}


float humedad(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  //Serial.println(F("Humidity: "));
  //Serial.println(h);
  //return round(h);
  return(h);
 }

float temperatura(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  //Serial.println(F("Temperature: "));
  //Serial.println(t);
  //return round(t);
  return(t);
}
