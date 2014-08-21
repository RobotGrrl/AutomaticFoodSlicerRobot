/*
 * Automatic Food Slicer Robot
 * ----------------------------
 *
 * End Effector Code
 *
 * Erin K / RobotGrrl.com
 * Sunday, August 10, 2014
 *
 */
 
#include <Streaming.h>
#include <Servo.h>
#include "Promulgate.h"
#include <SoftwareSerial.h>

// -- pins
int status_led = 11;
int servo1 = 9;
int tx = 7;
int rx = 8;

Servo knifey;
SoftwareSerial mega(rx, tx);

Promulgate promulgate = Promulgate(&mega, &mega);


// -- servo
int homepos = 90;


// -- misc
long last_led_blink = 0;


// -- state
#define ALL_OFF 0
#define MOVING 1
#define CUTTING 2
int CURRENT_STATE = 0;


// -- testing
long last_step = 0;
int current_step = 0;


// --
int servo_pos = 90;



void setup() {

  Serial.begin(9600);
  mega.begin(9600);
  
  
  promulgate.set_debug_stream(&Serial);
  promulgate.LOG_LEVEL = Promulgate::ERROR_;
  promulgate.set_rx_callback(received_action);
  promulgate.set_tx_callback(transmit_complete);

  promulgate.transmit_action('@', 'H', 1, 1, '!');
  
  
  move_knifey(homepos);
  pinMode(status_led, OUTPUT);
  
  /*
  for(int i=0; i<180; i++) {
    Serial << i << endl;
    knifey.write(i);
    delay(10);
  }
  
  knifey.write(homepos);
  delay(1000);
  
  knifey.detach();
  */
  
}


void loop() {
  
  //runStateMachineTest();
  
  
  if(mega.available()) {
    char c = mega.read();
    promulgate.organize_message(c);
  }
  
  
  if(Serial.available()) {
    char c = Serial.read();
    promulgate.organize_message(c);
  }
  
  
  switch(CURRENT_STATE) {
    
    case ALL_OFF: { // 0
      
      knifey.detach();
      
      if(millis()-last_led_blink > 1000) {
        digitalWrite(status_led, !digitalRead(status_led));
        last_led_blink = millis();
      }
      
    }
    break;
    
    case MOVING: { // 1
      
      if(millis()-last_led_blink > 100) {
        digitalWrite(status_led, !digitalRead(status_led));
        last_led_blink = millis();
        knifey.write(servo_pos);
      }
      
    }
    break;
    
    case CUTTING: { // 2
      
      digitalWrite(status_led, HIGH);
      
    }
    break;
    
  }
  
  
}




void received_action(char action, char cmd, uint8_t key, uint16_t val, char delim) {
  Serial << "---CALLBACK---" << endl;
  Serial << "action: " << action << endl;
  Serial << "command: " << cmd << endl;
  Serial << "key: " << key << endl;
  Serial << "val: " << val << endl;
  Serial << "delim: " << delim << endl;
  
  if(action == '@') {
    
    switch(cmd) {
      
      // Knife
      case 'K': {
      
        if(CURRENT_STATE == ALL_OFF) return; // has to be in moving or cutting state
        
        if(val >= 0 && val <= 180) {
          move_knifey(val);
        }
        
      }
      break;
      
      // Estop
      case 'E': {
        
        CURRENT_STATE = ALL_OFF;
        
      }
      break;
     
      // State
      case 'S': {
        
        if(key == 1) {
          CURRENT_STATE = MOVING;
        } else if(key == 2) {
          CURRENT_STATE = CUTTING;
        } else if(key == 0) {
          CURRENT_STATE = ALL_OFF;
        }/* else {
          CURRENT_STATE = ALL_OFF;
        }*/
        
      }
      break;
      
    }
    
  }
  
}



void move_knifey(int pos) {
  knifey.attach(servo1);
  knifey.write(pos);
  delay(50);
  servo_pos = pos;
}



void transmit_complete() {
  
}


