#include <Arduino_FreeRTOS.h>
void setup() {
  DDRB = B00000000; 
  xTaskCreate(taskled1, "Seq Led 1 Task", 128, NULL, 1, NULL);
  //xTaskCreate(taskled2, "Seq Led 2 Task", 128, NULL, 1, NULL);
  Serial.begin(9600);
}

void loop() {

}

void taskled1 (void *pvParameters){
   DDRB =  B00001111;
   int bandera=0;
   PORTB = B00001000;
   vTaskDelay (500 / portTICK_PERIOD_MS);
   while(1){
    if(bandera == 0){
      PORTB = PORTB >> 1;
      vTaskDelay (500 / portTICK_PERIOD_MS);
    }
    if(PORTB == B00000001 || bandera==1){
      bandera = 1;
      PORTB = PORTB << 1;
      vTaskDelay (500 / portTICK_PERIOD_MS);
      if (PORTB == B00001000){
        bandera=0;
        PORTB = B00001000;
      }
    }
    
  }
   
}
