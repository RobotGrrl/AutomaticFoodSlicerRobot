/*
 * Automatic Food Slicer Robot
 * ---------------------------
 *
 * Z-Axis Carriage Code (Motor + Encoder)
 *
 * Erin K / RobotGrrl.com
 * Thursday, July 3, 2014
 *
 */

#include <SPI.h>
#include "Adafruit_BLE_UART.h"
#include <Streaming.h>
#include "Promulgate.h"
#include <Wire.h>
#include <Hover.h>

#define ADAFRUITBLE_REQ 24
#define ADAFRUITBLE_RDY 25
#define ADAFRUITBLE_RST 26

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

Promulgate endeffector(&Serial2, &Serial2);

Promulgate promulgate = Promulgate(&Serial, &Serial);

uint8_t ACTION_STATE = 0;
uint8_t PREV_ACTION_STATE = 0;

long startActionTime = 0;
boolean pressed_buttons[] = {false, false, false, false};


boolean running_routine = false;
uint8_t frame_num = 0;
uint8_t prev_frame_num = 0;
uint8_t routine_num = 0;

// --- pins

int led1 = 34;
int status_led = 13;
int pos_led = 32; // red
int neg_led = 33; // blue

int m1_pin1 = 30;
int m1_pin2 = 31;
int m1_enable = 11;

int motor_r_pin1 = 40;
int motor_r_pin2 = 41;
int motor_r_enable = 9;

int motor_l_pin1 = 42;
int motor_l_pin2 = 43;
int motor_l_enable = 10;

int light = 12;
int spkr = 23;

int ldr = A0;
int button_bottom_l = A1;
int button_bottom_r = A2;
int button_top_l = A3;
int button_top_r = A4;

int bottom_led = 44;
int top_led = 45;

int ee_estop = 35;
int ee_resistor = A5;


uint8_t ts = 5;
uint8_t reset = 6;
uint8_t led = 13;

// -- hover
Hover hover = Hover();
byte event;



// --- BLE related
aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;
char msg_payload[20];
uint8_t msg_len = 0;

uint8_t outgoing_data_len = 9;
uint8_t outgoing_data[9] = {'#', 'A', '1', ',', '9', '9', '9', '9', '!'};

long last_send = 0;
int count = 0;

uint16_t given_on_time = 0;


// --- encoder
int avg_count = 7;
int total_count = 0;
int cur_val = 0;
int prev_val = 0;
int total_val = 0;
float avg_val = 0;
int read_vals[5];

int thresh = 20;
int raw_thresh = 10;

int enc_ticks = 0;

boolean tick = true;
int thresh1 = 200-5; // greater than
int thresh2 = 90+5; // less than


// --- motor
int motor_speed = 255;
int motor_dir = 0; // 0: release, 1: forewards, 2: backwards
boolean motor_direction_up = true;

int drive_motor_speed = 128;
int drive_motor_dir = 0;  // 0: release, 1: forewards, 2: backwards
boolean drive_motor_fwd = true;


// --- encoder
boolean counting = false;
int start_tick = 0;
long test_start = 0;
long test_end = 0;
int tick_goal = 2;
boolean tick_holding = false;
int tick_hold_count = 0;
long hold_tick_time = 0;
boolean dirh = true;
int last_hold_tick = 0;

long last_bottom_press = 0;
long last_top_press = 0;


// --- encoder 2
int lowest_val = 999;
int highest_val = 0;
int all_vals[20];
long evaluate_tick = 0;
long sig_tick = 0;


// -- end effector
int end_effector_type = 0; // green knife by default because this is the only one so far
boolean currentEEState = false;
long last_ee_cmd = 0;
int eePos = 90;




long last_hover = 0;


void setup() {

  Serial.begin(9600);
  Serial2.begin(9600);
  while(!Serial);
  Serial << "hello!" << endl;
  
  promulgate.set_rx_callback(ble_received_action);
  
  endeffector.set_rx_callback(ee_received_action);
  endeffector.set_tx_callback(ee_transmit_complete);
  
  pinMode(light, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(ldr, INPUT);
  pinMode(pos_led, OUTPUT);
  pinMode(neg_led, OUTPUT);
  pinMode(m1_pin1, OUTPUT);
  pinMode(m1_pin2, OUTPUT);
  pinMode(m1_enable, OUTPUT);
  
  pinMode(motor_r_pin1, OUTPUT);
  pinMode(motor_r_pin2, OUTPUT);
  pinMode(motor_r_enable, OUTPUT);
  
  pinMode(motor_l_pin1, OUTPUT);
  pinMode(motor_l_pin2, OUTPUT);
  pinMode(motor_l_enable, OUTPUT);
  
  pinMode(button_bottom_l, INPUT);
  pinMode(button_bottom_r, INPUT);
  pinMode(button_top_l, INPUT);
  pinMode(button_top_r, INPUT);
  pinMode(status_led, OUTPUT);
  
  pinMode(bottom_led, OUTPUT);
  pinMode(top_led, OUTPUT);
  
  pinMode(spkr, OUTPUT);
  
  BTLEserial.begin();
  
  analogWrite(light, 255);
  delay(100);
  
  for(int i=0; i<5; i++) {
    read_vals[i] = 0;
  }
  
  for(int i=0; i<20; i++) {
    all_vals[i] = 0;
  }
  
  motor_run(0, 0);
  
  
  digitalWrite(led1, HIGH);
  delay(1000);
  hover.begin(ts, reset);
  delay(100);
  digitalWrite(led1, LOW);
  
  
  /*
  while(true) {
    drive_motor_run(1, 255);
    delay(1000);
    drive_motor_run(2, 255);
    delay(1000);
    drive_motor_run(0, 0);
    delay(1000);
  }
  */
  
  robotgrrlSong();
  
}

void loop() {
  
  /*
  // ---
  BTLEserial.pollACI();
  ble_check_state();
  
  
  // ---
  if(running_routine) {
    routine_machine();
  } else {
    state_machine();  
  }
  
  
  // ---
  check_buttons();
  evaluate_encoder();
  */
  
  check_buttons();
  hoverDemo();
  
  eeUpdate();
  
}




void eeUpdate() {
  
  if(millis()-last_ee_cmd > 5000) {
    eeCutting(false);
  }
  
}

void eeSetKnifePos(int pos) {
  
  Serial << "setting ee knife pos: " << pos << endl;
  endeffector.transmit_action('@', 'K', 1, pos, ';');
  eePos = pos;
  
}

void eeCutting(boolean b) {
  
  if(b == false && currentEEState == true) {
    // if we want it off and it is on, turn it off!
    Serial << "turning ee off" << endl;
    endeffector.transmit_action('@', 'S', 0, 0, ';');
    currentEEState = false;
    
  } else if(b == true && currentEEState == false) {
    // if we want it on and it is off, turn it on!
    Serial << "turning ee on" << endl;
    endeffector.transmit_action('@', 'S', 2, 0, ';');
    currentEEState = true;
    
  }
  
}




void check_buttons() {
 
  boolean pressy = false;
  
  if(millis()-last_bottom_press >= 500) {
    
    pressy = false;
    
    if(digitalRead(button_bottom_l) == HIGH) {
      //Serial << "bottom left pressed" << endl;
      updateButtonLabel(3, 1);
      pressed_buttons[3-1] = true;
      pressy = true;
    } else {
      if(pressed_buttons[3-1]) {
        updateButtonLabel(3, 0);
        pressed_buttons[3-1] = false;
      }
    }
   
    if(digitalRead(button_bottom_r) == HIGH) {
      //Serial << "bottom right pressed" << endl;
      updateButtonLabel(4, 1);
      pressed_buttons[4-1] = true;
      pressy = true;
    } else {
      if(pressed_buttons[4-1]) {
        updateButtonLabel(4, 0);
        pressed_buttons[4-1] = false;
      }
    }
    
    if(pressy) {
      Serial << "bottom pressed" << endl; 
      digitalWrite(bottom_led, HIGH);
      last_bottom_press = millis();
      
      if(ACTION_STATE == 1 || ACTION_STATE == 2) {
        state_machine_stop();
      }
      
    } else {
      digitalWrite(bottom_led, LOW); 
    }
    
  }
  
  if(millis()-last_top_press >= 500) {
  
    pressy = false;
    
    if(digitalRead(button_top_l) == HIGH) {
      //Serial << "top left pressed" << endl;
      updateButtonLabel(1, 1);
      pressed_buttons[1-1] = true;
      pressy = true;
    } else {
      if(pressed_buttons[1-1]) {
        updateButtonLabel(1, 0);
        pressed_buttons[1-1] = false;
      }
    }
      
    if(digitalRead(button_top_r) == HIGH) {
      //Serial << "top right pressed" << endl;
      updateButtonLabel(2, 1);
      pressed_buttons[2-1] = true;
      pressy = true;
    } else {
      if(pressed_buttons[2-1]) {
        updateButtonLabel(2, 0);
        pressed_buttons[2-1] = false;
      }
    }
    
    if(pressy) {
      Serial << "top pressed" << endl;
      digitalWrite(top_led, HIGH);
      last_top_press = millis();
      
      if(ACTION_STATE == 1 || ACTION_STATE == 2) {
        state_machine_stop();
      }
      
    } else {
      digitalWrite(top_led, LOW); 
    }
  
  }
  
}






void ticks_go(int t, boolean dir) {
  
  tick_goal = t;
      
  start_tick = enc_ticks;
  counting = true;
  test_start = millis();
  
  Serial << "start cur_val: " << cur_val << endl;
  
  if(dir) {
    motor_run(1, motor_speed);  
  } else {
    motor_run(2, motor_speed);
  }
  
}







void motor_run(int mode, int speedy) {
  if(mode == 1) {
    // forward
    digitalWrite(m1_pin1, HIGH);
    digitalWrite(m1_pin2, LOW);
  } else if(mode == 2) {
    // backward
    digitalWrite(m1_pin1, LOW);
    digitalWrite(m1_pin2, HIGH);
  } else {
    digitalWrite(m1_enable, LOW);
  }
  motor_dir = mode;
  analogWrite(m1_enable, speedy);
}




void drive_motor_run(int mode, int speedy) {
  
  if(mode == 1) {
    // forward
    digitalWrite(motor_r_pin1, HIGH);
    digitalWrite(motor_r_pin2, LOW);
    
    digitalWrite(motor_l_pin1, HIGH);
    digitalWrite(motor_l_pin2, LOW);
    
  } else if(mode == 2) {
    // backward
    digitalWrite(motor_r_pin1, LOW);
    digitalWrite(motor_r_pin2, HIGH);
    
    digitalWrite(motor_l_pin1, LOW);
    digitalWrite(motor_l_pin2, HIGH);
    
  } else {
    digitalWrite(motor_r_enable, LOW);
    digitalWrite(motor_l_enable, LOW);
  }
  
  drive_motor_dir = mode;
  analogWrite(motor_r_enable, speedy);
  analogWrite(motor_l_enable, speedy);
  
}



void evaluate_encoder() {
  
  push_val(cur_val);
  prev_val = cur_val;
  cur_val = analogRead(ldr);
  analogWrite(led1, cur_val);
  
  /*
  if(abs(prev_val-cur_val) >= thresh) {
      encoder_tick();
      
      //if(cur_val >= thresh1 || cur_val <= thresh2) {
      //  encoder_tick(); 
      //}
      
    }
  */
  
  if(cur_val >= thresh1 && tick == false) {
    encoder_tick();
    tick = true;
  } else if(cur_val <= thresh2 && tick == true) {
    encoder_tick();
    tick = false;
  }
  
}


void encoder_tick() {
 
  enc_ticks++;
  Serial << " tick!" << enc_ticks << endl;
  //Serial << "avg_val: " << avg_val << endl;
  Serial << "cur_val: " << cur_val << endl;
  if(enc_ticks%2 == 0) {
    digitalWrite(pos_led, HIGH);
    digitalWrite(neg_led, LOW);
  } else {
    digitalWrite(pos_led, LOW);
    digitalWrite(neg_led, HIGH);
  }
  
  updateEncoderLabel(!tick);
  
}


void push_val(int v) {
  
  read_vals[4] = read_vals[3];
  read_vals[3] = read_vals[2];
  read_vals[2] = read_vals[1];
  read_vals[1] = read_vals[0];
  read_vals[0] = v;
  
}





// -- SPEAKER -- //
void robotgrrlSong() {
  
  Serial << "WFT" << endl;
  
  for(int i=0; i<5; i++) {
    playTone(260, 70);
    playTone(280, 70);
    playTone(300, 70);
    delay(100);
  } 
}

void playTone(int tone, int duration) {
	
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(spkr, HIGH);
    delayMicroseconds(tone);
    digitalWrite(spkr, LOW);
    delayMicroseconds(tone);
  }
	
}

