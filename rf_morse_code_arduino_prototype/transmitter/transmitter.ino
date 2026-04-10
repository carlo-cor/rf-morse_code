#include <RH_ASK.h>
#include <SPI.h>

#define TX 2
#define DOT_INPUT 3
#define DASH_INPUT 4
#define ENTER 5

RH_ASK rf_driver(2000, 0, TX);
String msg;
uint8_t bit_input[5];
uint8_t buffer_count = 0;

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

void setup() {
  // put your setup code here, to run once:
  rf_driver.init();
  Serial.begin(9600);
}
/*  ============================================================= GENERAL NOTES =========================================================================
//  - For now let's represent 0's as dashes and 1's as dots, however, once we successfully  test this brute force code try to challenge yourself with the ASCII/chars
//  - If I need semaphores, maybe consider using FreeRTOS to get a practice feel for it for work but also for the first phase of the prototype I think it 
//    be done without
//  - Look into what exact MCU you want to use, a Pico might be overkill for this so as of right now I'm thinking of sticking to the AtMega328P MCU on the Nano for the
//    KiCad PCB
*/

void loop() {
  Serial.println("Enter a number: ");
  while (!Serial.available() && btn_overwrite == false) {
    // If no Serial data comes in through the Serial Monitor, read button inputs
    // Add in proper debouncing later
    delay(100);
    dot_state = digitalRead(DOT_INPUT);
    dash_state = digitalRead(DASH_INPUT);
    enter_state = digitalRead(ENTER);

    // Serial.println(enter_state);
    
    // If the enter button hasn't been pressed and the buffer isn't full, allow for dash and dot inputs
    if (enter_state == LOW && buffer_count < 5) {
      if (dash_state == HIGH && last_dash_in_state == LOW) {
        msg[buffer_count] = '0';
        buffer_count++;
      } else if (dot_state == HIGH && last_dot_in_state == LOW) {
        msg[buffer_count] = '1';
        buffer_count++;
      }
    } 
    // Otherwise, if the enter button is toggled or the buffer gets full, force break and move on
    else if((enter_state == HIGH && last_enter_state == LOW) || buffer_count > 5) {
      buffer_count = 0;
      btn_overwrite = true;
    }
    // Always record the last state of each button for toggle implementation
    last_dot_in_state = dot_state;
    last_dash_in_state = dash_state;
    last_enter_state = enter_state;
  }

  // If Serial data does come through modify button overwrite to false and reset the buffer_count
  if (Serial.available()) {
    buffer_count = 0;
    btn_overwrite = false;
  }

  // If button overwrite is true, validify
  if (btn_overwrite) {
    valid = true;
  } else {
    msg = Serial.readStringUntil('\n');
    msg.trim();

    Serial.println("Trying to send LED code message: " + msg);
    Serial.println("Length of message: " + String(msg.length()));
  }

  if (msg.length() > 5) {
    Serial.println("Length too long try again.");
    valid = false;
  }

  // Otherwise parse it through
  else {
    for (int i = 0; i < msg.length(); i++) {
      char c = msg.charAt(i);
      if (c == '0' || c == '-') {
        bit_input[i] = 0;
      } else if (c == '1' || c == '.') {
        bit_input[i] = 1;
      } else {
        valid = false;
        break;
      }
    }
    valid = true;
  }

  if (valid == true) {
    Serial.print("LED Code Checksum: ");
    for (int i = 0; i < 5; i++) {
      if (bit_input[i]) {
        Serial.print('1');
      } else {
        Serial.print('0');
      }
    }

    rf_driver.send(bit_input, msg.length());
    rf_driver.waitPacketSent();
    delay(1000);

    Serial.println("\nLED Code Sent: " + msg);
  } else {
    Serial.println("Invalid codeword, try again");
  }
}
