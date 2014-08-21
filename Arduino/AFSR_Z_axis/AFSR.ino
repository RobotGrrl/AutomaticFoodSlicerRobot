void initialLoad() {
  
  // send some dummy data, because for some reason the ipad
  // doesn't listen the first few times when the app connects
  
  for(int i=0; i<5; i++) {
    ble_send_message('^', 'H', 1, 1, ';');
    delay(10);
  }
  
  updateMotorSpeed();
  delay(10);
  updateEnc1Thresh();
  delay(10);
  updateEnc2Thresh();
  delay(10);
  updateEncoderLabel(tick);
  delay(10);
  
  for(int i=1; i<=4; i++) {
    updateButtonLabel(i, 0);
    delay(10);
  }
}


// ---

void goToTop() {
  
  ACTION_STATE = 1;
  
  motor_run(1, motor_speed);
  
}

void goToBottom() {
  
  ACTION_STATE = 2;
  
  motor_run(2, motor_speed);
  
}


void stepUp() {
  
  ACTION_STATE = 3;
  
  if(enc_ticks-start_tick == tick_goal && counting == true) {
    motor_run(0, 0);
    
    test_end = millis();
    counting = false;
    
    Serial << "end cur_val: " << cur_val << endl;
    Serial << "elapsed time (ms): " << test_end-test_start << endl;
    
    state_machine_stop();
  }
  
  if(counting) {
    Serial << "cur_val: " << cur_val << endl;
  }
  
}

void stepDown() {
  
  ACTION_STATE = 4;
  
  if(enc_ticks-start_tick == tick_goal && counting == true) {
    motor_run(0, 0);
    
    test_end = millis();
    counting = false;
    
    Serial << "end cur_val: " << cur_val << endl;
    Serial << "elapsed time (ms): " << test_end-test_start << endl;
    
    state_machine_stop();
  }
  
  if(counting) {
    Serial << "cur_val: " << cur_val << endl;
  }
  
}


void upTime() {
  
  ACTION_STATE = 5;
  
  if(millis()-startActionTime < given_on_time) {
    Serial << "current: " << millis() << endl;
    motor_run(1, motor_speed);
  } else {
    Serial << "telling it to stop" << endl;
    state_machine_stop();
  }
  
}

void downTime() {
  
  ACTION_STATE = 6;
  
  if(millis()-startActionTime < given_on_time) {
    Serial << "current: " << millis() << endl;
    motor_run(2, motor_speed);
  } else {
    Serial << "telling it to stop" << endl;
    state_machine_stop();
  }
  
}


void releaseButton() {
  ACTION_STATE = 0;
}

void holdButton() {
  ACTION_STATE = 7;
  
  if(tick_holding) {
    
    if(enc_ticks-last_hold_tick > 0) {// && millis()-hold_tick_time >= 50) {
      Serial << "ting!" << endl;
      dirh = !dirh;
      //hold_tick_time = millis();
      last_hold_tick = enc_ticks;
    }
    
    if(dirh) {
      motor_run(1, motor_speed);
    } else {
      motor_run(2, motor_speed); 
    }
    
  }
  
}


void motorJog() {
  
}


// ---

void motorSpeedSlider(int val) {
  motor_speed = val;
  Serial << "new motor speed: " << motor_speed << endl;
}

void enc1Thresh(int val) {
  thresh1 = val;
  Serial << "new enc1 thresh: " << thresh1 << endl;
}

void enc2Thresh(int val) {
  thresh2 = val;
  Serial << "new enc2 thresh: " << thresh2 << endl;
}

void ledButton(int val) {
  if(val == 0) {
    digitalWrite(light, LOW);
  } else if(val == 1) {
    digitalWrite(light, HIGH);
  } else {
    analogWrite(light, val); 
  }
  Serial << "led button" << endl;
}

// ---

void updateMotorSpeed() {
  // #M1,255! 
  
  ble_send_message('#', 'M', 1, motor_speed, '!');
  
}

void updateEnc1Thresh() {
  // #N1,1023! 
  
  ble_send_message('#', 'N', 1, thresh1, '!');
  
}

void updateEnc2Thresh() {
  // #N2,1023! 
  
  ble_send_message('#', 'N', 2, thresh2, '!');
  
}

void updateEncoderLabel(boolean t) {
  // #E1,0! 
  
  if(t) {
    ble_send_message('#', 'E', 1, 1, '!');  
  } else {
    ble_send_message('#', 'E', 1, 0, '!');
  }
  
}

void updateButtonLabel(int b, int s) {
  // #B1,0!
  
  //int s = 0;
  
  /*
  switch(b) {
    case 0:
      
    break;
    case 1:
      if(digitalRead(button_top_l) == HIGH) {
        s = 1;
      } else {
        s = 0;
      }
    break;
    case 2:
      if(digitalRead(button_top_r) == HIGH) {
        s = 1;
      } else {
        s = 0;
      }
    break;
    case 3:
      if(digitalRead(button_bottom_l) == HIGH) {
        s = 1;
      } else {
        s = 0;
      }
    break;
    case 4:
      if(digitalRead(button_bottom_r) == HIGH) {
        s = 1;
      } else {
        s = 0;
      }
    break;
  }
  */
  
  ble_send_message('#', 'B', b, s, '!');
  
}


