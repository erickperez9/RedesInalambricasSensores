#include <Arduino.h>
#include <painlessMesh.h>
#include <WiFiClient.h>

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
#define HOSTNAME "MQTT_Bridge"

#define   STATION_SSID     "Mateo_Molina"
#define   STATION_PASSWORD "Thomas3117"



// Prototypes
void receivedCallback( const uint32_t &from, const String &msg );

IPAddress getlocalIP();
IPAddress myIP(0,0,0,0);
IPAddress myAPIP(0,0,0,0);


painlessMesh  mesh;
void setup() {
  Serial.begin(115200);

  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages

  // Channel set to 6. Make sure to use the same channel for your mesh and for you other
  // network (STATION_SSID)
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 8 );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  mesh.stationManual(STATION_SSID, STATION_PASSWORD);
  mesh.setHostname(HOSTNAME);

  myAPIP = IPAddress(mesh.getAPIP());
  Serial.println("MY AP IP is: " + myAPIP.toString());
  
}

void loop() {
  mesh.update();
  if(myIP != getlocalIP()){
    myIP = getlocalIP();
    Serial.println("My IP is " + myIP.toString());
  }
}

void receivedCallback( const uint32_t &from, const String &msg ) {
  Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
  delay(1000);
  
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
IPAddress getlocalIP() {
  return IPAddress(mesh.getStationIP());
}
