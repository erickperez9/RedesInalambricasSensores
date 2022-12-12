#include "base64.hpp"
#include <base64.h>


uint16_t hum = 50;
uint16_t temp = 20;
long timeStamp = 6;

char s[4];
String toEncode_trama="";
String toEncode_ts="";
String toEncode_temp="";
String toEncode_hum="";
String encoded_TS="";
String encoded_temp="";
String encoded_hum="";
String encoded_trama="";


typedef union{
  struct {
    uint32_t ts;
    uint32_t t:9;
    uint32_t h:7;
  };
  byte datos[6];  //linea para la recuperacion de bits 
}trama;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  trama valores;
  valores.ts=timeStamp;
  valores.t=temp;
  valores.h=hum;
  Serial.write(valores.datos,6);
  
  Serial.write(valores.ts);
  Serial.write(valores.t);
  Serial.write(valores.h);
  Serial.write(valores.datos,6); //Trama completa
  Serial.println(valores.ts);
  Serial.println(valores.t);
  Serial.println(valores.h);

  // *******************Codificacion*******************************
  
   toEncode_trama = String(valores.datos,6);
   toEncode_ts = String(valores.ts);
   toEncode_temp = String(valores.t);
   toEncode_hum = String(valores.h);
  encoded_TS = base64::encode(toEncode_ts); //codificacion time stamps
  encoded_temp = base64::encode(toEncode_temp); // codificacion temp
  encoded_hum = base64::encode(toEncode_hum); //codificacion hum
  encoded_trama = base64::encode(toEncode_trama); //codificacion trama completa
  
  /*
  snprintf(s, 4, "%d", valores.datos);
  Serial.println(s); */
  delay(1000);
  Serial.println("\n DATA ENCODED");
  Serial.print("Base64 Time Stamp: ");Serial.println(encoded_TS);
  Serial.print("Base64 Temp: ");Serial.println(encoded_temp);
  Serial.print("Base64 Hum: ");Serial.println(encoded_hum);
  Serial.print("Base64 Trama comp: ");Serial.println(encoded_trama);


 

  
  
}
