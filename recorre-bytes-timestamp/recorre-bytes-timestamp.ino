
uint16_t hum = 50;
uint16_t temp = 20;
long timeStamp = 6;
char a[8];

uint16_t humRX;
uint16_t tempRX;
uint16_t timeStampRX;
char aRX[8];

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  a[0] = (char)(timeStamp >> 24);
  a[1] = (char)(timeStamp >> 16);
  a[2] = (char)(timeStamp >> 8);
  a[3] = (char)(timeStamp);
  a[4] = (char)(temp >> 8);
  a[5] = (char)(temp);
  a[6] = (char)(hum >> 8);
  a[7] = (char)(hum);
  Serial.println(" ");
  Serial.println("*** TX ***");
  Serial.write(a,8);
  Serial.println(a);
  delay(1000);
  Serial.println("\n");

  timeStampRX = (uint32_t)(a[0]<<24 | a[1]<<16 | a[2]<<8 | a[3] );
  tempRX = (uint16_t)(a[4]<<8 | a[5]);
  humRX = (uint16_t)(a[6]<<8 | a[7]);
  Serial.println("*** RX ***");
  Serial.write(timeStampRX);
  Serial.write(tempRX);
  Serial.write(humRX);
  delay(1000);
  Serial.println("\n...");
}
