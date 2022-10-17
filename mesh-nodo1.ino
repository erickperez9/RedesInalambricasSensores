//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"
#include "DHT.h"
#include <WiFi.h>
#include <WiFiClient.h>

#define   MESH_PREFIX     "whateverYouLike111"
#define   MESH_PASSWORD   "somethingSneaky111"
#define   MESH_PORT       5555

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 13   

DHT dht(DHTPIN, DHTTYPE);
Scheduler userScheduler; // to control your personal task

painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
   //String msg = String(random(1,20));
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
  
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  mesh.update();
}


int humedad(){
  float h = dht.readHumidity();
  if ( isnan(h) ) {
    Serial.println(F("Failed to read Humidity from DHT sensor!"));
  }
  Serial.println(F("Humidity: "));
  Serial.print(h);
  return round(h);
 }

 int temperatura(){
  float t = dht.readTemperature();
  if ( isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  Serial.println(F("Temperature: "));
  Serial.print(t);
  return round(t);
}
