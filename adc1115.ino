#include <Adafruit_ADS1X15.h>
#include <Wire.h>
// Crear objeto de la clase
Adafruit_ADS1115 ads;

void setup(void) 
{
  Serial.begin(9600);
  delay(200);

  // Cambiar factor de escala
  ads.setGain(GAIN_ONE);
  
  // Iniciar el ADS1115
  ads.begin();
}

void loop(void) 
{
  // Obtener datos del A0 del ADS1115
  short adc0 = ads.readADC_SingleEnded(0);
  Serial.print("A0: "); 
  Serial.println(adc0);
 
  delay(1000);
}
