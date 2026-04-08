#include <RH_ASK.h>
#include <SPI.h>

#define DOT_INPUT 3
#define DASH_INPUT 4
#define TX 2

RH_ASK rf_driver(2000, 0, TX);

void setup() {
  // put your setup code here, to run once:
  rf_driver.init();
  Serial.begin(9600);
}
/*  ============================================================= GENERAL NOTES =========================================================================
//  - For now let's represent 0's as dashes and 1's as dots, however, once we transition from the Nano MCU try to challenge yourself with the ASCII/chars
//  - If I need semaphores, maybe consider using FreeRTOS to get a practice feel for it for work but also for the first phase of the prototype I think it 
//    be done without
*/

void loop() {
  String msg;
  uint8_t bit_input[5];
  bool btn_overwrite;
  bool valid = true;
  Serial.println("Enter a number: ");

  // I'm guessing that the Serial.available() function holds the resource through a mutex, might want to check later and then
  // implement my own semaphore control. Also can be checked later as I'll likely transition onto
  // a Pico SDK environment

  while(!Serial.available()){
    // Wait till data arrives from Serial Monitor input, if no data arrives allow for button inputs
    /*
    for(int i = 0; i < 5; i++){
      if(DASH_INPUT == 1){
        msg[i] = '0';
      }
      else if(DOT_INPUT == 1){
        msg[i] = '1';
      }
    }

    btn_overwrite = true;
    */
  }
  
  /*
  if(btn_overwrite){
    valid = true;
  }
  else{
    msg = Serial.readStringUntil('\n');
    msg.trim(); 
  }
  */
  
  msg = Serial.readStringUntil('\n');
  msg.trim(); 
  
  Serial.println("Trying to send LED code message: " + msg);
  Serial.println("Length of message: " + String(msg.length()));

  if (msg.length() > 5){
    Serial.println("Length too long try again.");
  }

  // Otherwise parse it through
  else{
    for(int i = 0; i < msg.length(); i++){
      char c = msg.charAt(i);
      if(c == '0' || c == '-'){
        bit_input[i] = 0; 
      }
      else if(c == '1' || c == '.'){
        bit_input[i] = 1;
      }
      else{
        valid = false;
        break;
      }
    }
    
    valid = true;
    
    if(valid == true){
      Serial.print("LED Code Checksum: ");
      for(int i = 0; i < 5; i++){
        if(bit_input[i]){
          Serial.print('1');
        }
        else{
          Serial.print('0');
        }
      }

      rf_driver.send(bit_input, msg.length());
      rf_driver.waitPacketSent();
      delay(1000);

      Serial.println("\nLED Code Sent: " + msg);
    }
    else{
      Serial.println("Invalid codeword, try again");
    }
  }
}
