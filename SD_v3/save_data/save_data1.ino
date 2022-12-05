/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <DHT_U.h>
#include <DHT.h>

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 15
DHT dht(DHTPIN, DHTTYPE);

String h="";
String t="";
String datos="";

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  //if(!file){
    //file.write((const uint8_t*)(message), sizeof(message));
    //Serial.println("Failed to open file for writing");
    //return;
  //}
  if(file)
  {
     Serial.println("abierto para escribir 'test.txt'");
     file.write((const uint8_t*)(&message), sizeof(message));
     file.close();
  }
  else{
    Serial.println("Failed to open file for writing");
  } 
  if(file.print(message)){
    //file.write((const uint8_t*)(message), sizeof(message));
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    
    return;
  }
  if(file.print(message)){
      //file.write((const uint8_t*)(&message), sizeof(message));
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void setup(){
    Serial.begin(115200);
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    
    
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
    
}

void loop(){
  uint16_t hum = 55;
  uint16_t temp = 23;
  long timestamp;
  char a[8];
  a[0] = (const char)(timestamp >> 24);
  a[1] = (const char)(timestamp >> 16);
  a[2] = (const char)(timestamp >> 8);
  a[3] = (const char)(timestamp);
  a[4] = (const char)(hum >> 8);
  a[5] = (const char)(hum);
  a[4] = (const char)(temp >> 8);
  a[5] = (const char)(temp);
  //float humedad = dht.readHumidity();
  //float temperatura = dht.readTemperature();
  //float a = 12.5;
  //h = String(humedad);
  //t = String(temperatura);
  //h.concat(t);
  //datos= "Humedad= "+h+" "+"Temperatura= "+t; 
  //String ad =String(a);
  writeFile(SD, "/test.txt", a);
  //appendFile(SD, "test.txt", a);
  Serial.println("Data Escrita");
  Serial.print("datos: ");
  Serial.print(a);
  delay(5000);
}
