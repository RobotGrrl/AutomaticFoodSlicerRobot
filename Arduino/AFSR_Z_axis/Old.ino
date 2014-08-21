
void evaluate_encoder2() {
 
  //push_val2(cur_val);
  prev_val = cur_val;
  cur_val = analogRead(ldr);
  analogWrite(led1, cur_val);
  
  
  if(cur_val <= lowest_val) {
    
    Serial << "cur_val <= lowest_val" << endl;
    lowest_val = cur_val;
    sig_tick = evaluate_tick;
    
  } else if(cur_val > lowest_val && cur_val < highest_val && tick == false) {
    
    if(evaluate_tick-sig_tick >= 2) {
      // let's assume it has ticked by now
      encoder_tick();
      highest_val = 0;
      tick = true;  
    }
    
  } else if(cur_val >= highest_val) {
    
    Serial << "cur_val >= highest_val" << endl;
    highest_val = cur_val;
    sig_tick = evaluate_tick;
    
  } else if(cur_val < highest_val && cur_val > lowest_val && tick == true) {
    
    if(evaluate_tick-sig_tick >= 2) {
      // let's assume it has ticked by now
      encoder_tick();
      lowest_val = 999;
      tick = false;  
    }
    
  }
  
  
  /*
  if(cur_val >= thresh1 && tick == false) {
    encoder_tick();
    tick = true;
  } else if(cur_val <= thresh2 && tick == true) {
    encoder_tick();
    tick = false;
  }
  */
  
  evaluate_tick++;
  
}

void push_val2(int v) {
  
  all_vals[19] = all_vals[18];
  all_vals[18] = all_vals[17];
  all_vals[17] = all_vals[16];
  all_vals[16] = all_vals[15];
  all_vals[15] = all_vals[14];
  all_vals[14] = all_vals[13];
  all_vals[13] = all_vals[12];
  all_vals[12] = all_vals[11];
  all_vals[11] = all_vals[10];
  all_vals[10] = all_vals[9];
  all_vals[9] = all_vals[8];
  all_vals[8] = all_vals[7];
  all_vals[7] = all_vals[6];
  all_vals[6] = all_vals[5];
  all_vals[5] = all_vals[4];
  all_vals[4] = all_vals[3];
  all_vals[3] = all_vals[2];
  all_vals[2] = all_vals[1];
  all_vals[1] = all_vals[0];
  all_vals[0] = v;
  
}






void blab() {
  
  if(motor_direction_up) {
    ticks_go(1, true);
  } else {
    ticks_go(1, false); 
  }
  
  
  
  if(millis()-last_bottom_press >= 500) {
    
    if(digitalRead(button_bottom_l) == HIGH || digitalRead(button_bottom_r) == HIGH) {
      Serial << "bottom pressed" << endl; 
      motor_direction_up = false;
      last_bottom_press = millis();
      //ticks_go(1, true);
    }
    
  }
  
  if(millis()-last_top_press >= 500) {
  
    if(digitalRead(button_top_l) == HIGH || digitalRead(button_top_r) == HIGH) {
      Serial << "top pressed" << endl;
      motor_direction_up = true;
      last_top_press = millis();
      //ticks_go(1, false);
    }
  
  }
  
  
  
  
  
  evaluate_encoder();
  //evaluate_encoder2();
  
  if(Serial.available() > 0) {
    char c = Serial.read();
    Serial << "> " << c << endl;
    if(c == ';') {
      
      ticks_go(2, true);
      
    } else if(c == 'l') {
      
      ticks_go(4, true);
      
    } else if(c == 'k') {
      
      if(motor_direction_up) {
        ticks_go(1, true);  
      } else {
        ticks_go(1, false); 
      }
      
    } else if(c == 'z') {
      
      tick_holding = true;
      motor_run(1, motor_speed);
      
    } else if(c == 'x') {
      
      tick_holding = false;
      motor_run(0, 0);
      
    } else if(c == '1') {
      thresh1 = cur_val-5;
      Serial << "set thresh1 to: " << thresh1 << endl; 
    } else if(c == '2') {
      thresh2 = cur_val+5;
      Serial << "set thresh2 to: " << thresh2 << endl;
    } else if(c == 'c') {
      Serial << "cur_val: " << cur_val << endl;
    }
  }
  
  
  if(enc_ticks-start_tick == tick_goal && counting == true) {
    motor_run(0, 0);
    
    delay(1000);
    
    test_end = millis();
    counting = false;
    
    Serial << "end cur_val: " << cur_val << endl;
    Serial << "elapsed time (ms): " << test_end-test_start << endl;
  }
  
  
  if(counting) {
    Serial << "cur_val: " << cur_val << endl;
  }
  
  
  if(tick_holding) {
    
    if(enc_ticks-last_hold_tick > 0) {// && millis()-hold_tick_time >= 50) {
      Serial << "ting!" << endl;
      dirh = !dirh;
      hold_tick_time = millis();
      last_hold_tick = enc_ticks;
    }
    
    if(dirh) {
      motor_run(1, motor_speed);
    } else {
      motor_run(2, motor_speed); 
    }
    
  }
  
  
}

