boolean firstpass = true;
boolean framefirstpass = true;

void routine_machine() {
 
 switch(routine_num) {
   case 0:
     if(firstpass) {
       prev_frame_num = 0;
       frame_num = 0;
       firstpass = false;
     }
     routine1();
   break;
 }
  
}

void routine1() {
  
  switch(frame_num) {
    
    case 0: {
    
      if(framefirstpass) {
        Serial << "frame " << frame_num << endl;
        motor_direction_up = true;
        framefirstpass = false;
      }
      
      goToTop();
      
      /*
      TODO: Fix when the state_machine_stop method is called, check for
      the button presses inside of this function instead
      
      if(pressed_buttons) {
        
      }
      */
      
    break;
    }
    
    case 1: {
    
      Serial << "A1" << endl;
    
      if(framefirstpass) {
        Serial << "A2" << endl;
        Serial << "frame " << frame_num << endl;
        motor_direction_up = false;
        given_on_time = 50;
        startActionTime = millis();
        Serial << "start time: " << startActionTime << endl;
        framefirstpass = false;
      }
      
      Serial << "A3" << endl;
      downTime();
      
    break;
    }
    
    case 2: {
      
      Serial << "A4" << endl;
      
      if(pressed_buttons[3-1] == true || pressed_buttons[4-1] == true) {
        
        frame_num = 4;
        
      } else {
      
        if(framefirstpass) {
          Serial << "A5" << endl;
          Serial << "frame " << frame_num << endl;
          motor_direction_up = true;
          given_on_time = 100;
          startActionTime = millis();
          framefirstpass = false;
        }
      
      }
      
      Serial << "A6" << endl;
      upTime();
      
    break;
    }
    
    case 3: {
      
      if(pressed_buttons[3-1] == true || pressed_buttons[4-1] == true) {
        
        frame_num = 4;
        
      } else {
        
        if(framefirstpass) {
          Serial << "frame " << frame_num << endl;
          motor_direction_up = false;
          given_on_time = 100;
          startActionTime = millis();
          framefirstpass = false;
        }
        
        downTime();
        
      }
    
    break;
    }
    
    case 4: {
    
      if(framefirstpass) {
        Serial << "frame " << frame_num << endl;
        motor_direction_up = true;
        given_on_time = 200;
        startActionTime = millis();
        framefirstpass = false;
      }
      
      upTime();
    
    break;
    }
    
    case 5: {
    
      if(framefirstpass) {
        Serial << "frame " << frame_num << endl;
        motor_direction_up = false;
        framefirstpass = false;
      }
      
      goToBottom();
    
    break;
    }
    
    case 6: {
    
      if(framefirstpass) {
        Serial << "frame " << frame_num << endl;
        motor_direction_up = true;
        given_on_time = 200;
        startActionTime = millis();
        framefirstpass = false;
      }
      
      upTime();
    
    break;
    }
    
    case 7: {
    
      if(framefirstpass) {
        Serial << "frame " << frame_num << endl;
        motor_direction_up = false;
        framefirstpass = false;
      }
      
      goToBottom();
    
    break;
    }
    
    case 8: {
    
      if(framefirstpass) {
        Serial << "frame " << frame_num << endl;
        motor_direction_up = true;
        framefirstpass = false;
        delay(1000);
      }
      
      goToTop();
    
    break;
    }
    
    case 9: {
    
      Serial << "frame " << frame_num << endl;
    
      for(int i=0; i<5; i++) {
        digitalWrite(pos_led, HIGH);
        delay(100);
        digitalWrite(pos_led, LOW);
        delay(100);
      }
    
      running_routine = false;
    
    break;
    }
  }
  
  if(frame_num != prev_frame_num) {
    prev_frame_num = frame_num;
  }
  
}
