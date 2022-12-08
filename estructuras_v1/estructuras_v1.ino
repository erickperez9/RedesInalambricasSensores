uint16_t hum = 50;
uint16_t temp = 20;
long timeStamp = 6;
//char a[8];
char s[4];

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
  
  /*
  snprintf(s, 4, "%d", valores.datos);
  Serial.println(s); */
  delay(1000);
  Serial.println("\n");

  
}
