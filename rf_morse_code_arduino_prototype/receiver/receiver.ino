#include <RH_ASK.h>
#include <SPI.h>

#define RX 2

#define LED1 7
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6

RH_ASK rf_driver(2000, RX, 0);

void setup() {
  rf_driver.init();
  Serial.begin(9600);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  char buffer[50];
  uint8_t led_code[5];
  uint8_t buflen = sizeof(led_code);

  if(rf_driver.recv(led_code, &buflen)){
    sprintf(buffer, "Message Received: %u", led_code);
    Serial.println(buffer);

    if(led_code[0] == 1){
      delay(50);
      digitalWrite(LED1, HIGH);
    }
    else{
      delay(50);
      digitalWrite(LED1, LOW);
    }

    if(led_code[1] == 1){
      delay(55);
      digitalWrite(LED2, HIGH);
    }
    else{
      delay(55);
      digitalWrite(LED2, LOW);
    }

    if(led_code[2] == 1){
      delay(60);
      digitalWrite(LED3, HIGH);
    }
    else{
      delay(60);
      digitalWrite(LED3, LOW);
    }

    if(led_code[3] == 1){
      delay(65);
      digitalWrite(LED4, HIGH);
    }
    else{
      delay(65);
      digitalWrite(LED4, LOW);
    }

    if(led_code[4] == 1){
      delay(70);
      digitalWrite(LED5, HIGH);
    }
    else{
      delay(70);
      digitalWrite(LED5, LOW);
    }    
  }
}
