#include <RH_ASK.h>
#include <SPI.h>

#define DOT_INPUT 3
#define DASH_INPUT 4
#define TX 2

RH_ASK rf_driver(2000, 0, TX);
<<<<<<< Updated upstream
=======
String msg;
uint8_t bit_input[5];
uint8_t buffer_count = 0;
bool button_in_accept_state;

// Declaration and definition of button states
uint8_t dot_state;
uint8_t dash_state;
uint8_t enter_state;

// Auto-default buttons to false or LOW
uint8_t last_dot_in_state = LOW;
uint8_t last_dash_in_state = LOW;
uint8_t last_enter_state = LOW;

// Button overwrite and validity checks
bool btn_overwrite = false;
bool valid = true;
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream

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
=======
  
  while (!Serial.available() && btn_overwrite == false) {
    // If no Serial data comes in through the Serial Monitor, read button inputs
    delay(100);
    dot_state = digitalRead(DOT_INPUT);
    dash_state = digitalRead(DASH_INPUT);
    enter_state = digitalRead(ENTER);

    // If the enter button hasn't been pressed and the buffer isn't full, allow for dash and dot inputs
    if (enter_state == LOW && buffer_count < 5) {
      if (dash_state == HIGH && last_dash_in_state == LOW) {
        bit_input[buffer_count] = 0;
        Serial.println("DASH BUTTON PRESSED!");
        Serial.println(bit_input[buffer_count]);
        buffer_count++;
      } else if (dot_state == HIGH && last_dot_in_state == LOW) {
        bit_input[buffer_count] = 1;
        Serial.println("DOT BUTTON PRESSED!");
        Serial.println(bit_input[buffer_count]);
        buffer_count++;
      }
    }
    // Otherwise, if the enter button is toggled or the buffer gets full, force break and move on
    else if ((enter_state == HIGH && last_enter_state == LOW) || buffer_count >= 5) {
      Serial.println("CHECK 3!");
      buffer_count = 0;
      btn_overwrite = true;
    } else {
      buffer_count = 0;
      Serial.println("CHECK 4!");
      break;
>>>>>>> Stashed changes
    }

<<<<<<< Updated upstream
    btn_overwrite = true;
    */
=======
  // Serial.print("\nLED CODE Entered Through Buttons: ");

  // If Serial data does come through modify button overwrite to false and reset the buffer_count
  if (Serial.available()) {
    buffer_count = 0;
    btn_overwrite = false;
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
  if (msg.length() > 5){
=======
  if (msg.length() > 5 && !btn_overwrite) {
>>>>>>> Stashed changes
    Serial.println("Length too long try again.");
  }

  // Otherwise parse it through
<<<<<<< Updated upstream
  else{
    for(int i = 0; i < msg.length(); i++){
=======
  else if (!btn_overwrite) {
    for (int i = 0; i < msg.length(); i++) {
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======

    rf_driver.send(bit_input, 5);
    rf_driver.waitPacketSent();
    delay(1000);

    Serial.println("\nLED Code Sent!");
  } else {
    Serial.println("Invalid codeword, try again");
>>>>>>> Stashed changes
  }

  btn_overwrite = false;
}