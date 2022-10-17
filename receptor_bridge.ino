#include <Arduino.h>
#include <painlessMesh.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#define   MESH_PREFIX     "whateverYouLike111"
#define   MESH_PASSWORD   "somethingSneaky111"
#define   MESH_PORT       5555
#define HOSTNAME "MQTT_Bridge"

/*#define   STATION_SSID     "Mateo_Molina"
#define   STATION_PASSWORD "Thomas3117"
#define   STATION_SSID     "Familia_Perez"
#define   STATION_PASSWORD "e#74-25_Pe$"*/
#define   STATION_SSID     "Erick"
#define   STATION_PASSWORD "tnbt8311+"

#define TOKEN "dyovhh14Giyz880RHqGD"
//char thingsboardServer[] = "192.168.18.189";
char thingsboardServer[] = "192.168.43.26";
/*#define TOKEN     "05npIe4dBvrcxGd2RI5W"
char thingsboardServer[] = "demo.thingsboard.io";*/

// Prototypes
void receivedCallback( const uint32_t &from, const String &msg );
void mqttCallback(char* topic, byte* payload, unsigned int length);

IPAddress getlocalIP();

IPAddress myIP(0,0,0,0);
IPAddress myAPIP(0,0,0,0);
IPAddress mqttBroker(0, 0, 0, 0);

long t;
long h;

painlessMesh  mesh;
WiFiClient wifiClient;
//PubSubClient mqttClient(mqttBroker, 1883, mqttCallback, wifiClient);
PubSubClient mqtt(wifiClient);
void setup() {
  Serial.begin(115200);

  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages

  // Channel set to 6. Make sure to use the same channel for your mesh and for you other
  // network (STATION_SSID)
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 11 );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  mesh.stationManual(STATION_SSID, STATION_PASSWORD);
  mesh.setHostname(HOSTNAME);

  /* Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
  mesh.setRoot(true);
  // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
  mesh.setContainsRoot(true);*/

  myAPIP = IPAddress(mesh.getAPIP());
  Serial.println("MY AP IP is: " + myAPIP.toString());
  
  mqtt.setServer(thingsboardServer, 1883);
  delay(10);
}

void loop() {
  mesh.update();
  mqtt.loop();
  if(myIP != getlocalIP()){
    myIP = getlocalIP();
    Serial.println("My IP is " + myIP.toString());
    /*
    if (mqttClient.connect("painlessMeshClient")) {
      mqttClient.publish("painlessMesh/from/gateway","Ready!");
      mqttClient.subscribe("painlessMesh/to/#");
    } 
    */
  }
}

void receivedCallback( const uint32_t &from, const String &msg ) {
  Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
  delay(1000);
  String topic = "painlessMesh/from/" + String(from);
  /*mqttClient.publish(topic.c_str(), msg.c_str());*/
  String t = msg.substring(0,2);
  String h = msg.substring(3,5);
  
  if (mqtt.connected() == false) {
    Serial.print("MQTT connection... ");
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

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}
/*
void mqttCallback(char* topic, uint8_t* payload, unsigned int length) {
  char* cleanPayload = (char*)malloc(length+1);
  memcpy(cleanPayload, payload, length);
  cleanPayload[length] = '\0';
  String msg = String(cleanPayload);
  free(cleanPayload);

  String targetStr = String(topic).substring(16);

  if(targetStr == "gateway")
  {
    if(msg == "getNodes")
    {
      auto nodes = mesh.getNodeList(true);
      String str;
      for (auto &&id : nodes)
        str += String(id) + String(" ");
      mqtt.publish("painlessMesh/from/gateway", str.c_str());
    }
  }
  else if(targetStr == "broadcast") 
  {
    mesh.sendBroadcast(msg);
  }
  else
  {
    uint32_t target = strtoul(targetStr.c_str(), NULL, 10);
    if(mesh.isConnected(target))
    {
      mesh.sendSingle(target, msg);
    }
    else
    {
      mqtt.publish("painlessMesh/from/gateway", "Client not connected!");
    }
  }
}*/

IPAddress getlocalIP() {
  return IPAddress(mesh.getStationIP());
}
