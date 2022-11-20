#include "painlessMesh.h"
#include "DHT.h"
#include <ThingsBoard.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
#define HOSTNAME "MQTT_Bridge"

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 13   

#define TOKEN               "prueba_test_wsn"
char thingsboardServer[] = "10.124.233.4";

IPAddress getlocalIP();
IPAddress myIP(0,0,0,0);
IPAddress myAPIP(0,0,0,0);

DHT dht(DHTPIN, DHTTYPE);
Scheduler userScheduler; 

painlessMesh  mesh;
WiFiClient wifiClient;

PubSubClient mqtt(wifiClient);

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String temp = String(temperatura());
  String hum = String(humedad());
  if( temp.length() == 1){
    temp = "0"+temp;
  }
  String msg = temp + " " + hum + " - ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 1 ));
  Serial.println(msg);
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(10);
  
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 8 );
  
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  mqtt.setServer(thingsboardServer, 1883);
  delay(10);
}

void loop() {
  mesh.update();
  if(myIP != getlocalIP()){
    myIP = getlocalIP();
    Serial.println("My IP is " + myIP.toString());
  }
  txmqtt();
  mqtt.loop();
}


int humedad(){
  float h = dht.readHumidity();
  if ( isnan(h) ) {
    Serial.println(F("Failed to read Humidity from DHT sensor!"));
  }
  Serial.println(F("Humidity: "));
  Serial.println(h);
  return round(h);
 }

 int temperatura(){
  float t = dht.readTemperature();
  if ( isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  Serial.println(F("Temperature: "));
  Serial.println(t);
  return round(t);
}

void txmqtt(){
  String t = String(temperatura());
  String h = String(humedad());
  if (mqtt.connected() == false) {
    Serial.println("MQTT connection... ");
    if (mqtt.connect(HOSTNAME, TOKEN, "")) {
      Serial.println("connected");
    } else {
      Serial.println("failed");
      delay(1000);
    }
  } else {
    String dataJS = "{\"TEMPERATURA\":" + t + ",\"HUMEDAD\":" + h + "}";
    char json[100];
    dataJS.toCharArray(json,dataJS.length()+1);
    mqtt.publish("v1/devices/me/telemetry", json);
    delay(1000);
  }

}
IPAddress getlocalIP() {
  return IPAddress(mesh.getStationIP());
}
