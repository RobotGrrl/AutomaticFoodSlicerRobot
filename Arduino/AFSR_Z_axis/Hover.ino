void hoverDemo() {
 
  if (hover.getStatus(ts) == 0) {
    
    //Get the event over i2c and print it
    event = hover.getEvent();
    Serial << hover.getEventString(event);
    
    //if(millis()-last_hover < 500) return; // probably an accident if they triggered within 1/2 second
    
    //last_hover = millis();
    
    switch(event) {
      
      case RIGHT_SWIPE: {
        Serial << "right swipe!" << endl;
        
        playTone(200, 70);
        delay(10);
        playTone(220, 70);
        
        drive_motor_run(2, 255);
        delay(300);
        drive_motor_run(0, 0);
      }
      break;
      
      case LEFT_SWIPE: {
        Serial << "left swipe!" << endl;
        
        playTone(250, 70);
        delay(10);
        playTone(270, 70);
        
        drive_motor_run(1, 255);
        delay(300);
        drive_motor_run(0, 0);
      }
      break;
      
      case UP_SWIPE: {
        
        if(pressed_buttons[2-1] == false && pressed_buttons[1-1] == false) {
      
          Serial << "up swipe!" << endl;
          
          playTone(300, 70);
          delay(10);
          playTone(320, 70);
          
          motor_run(1, motor_speed);
          delay(700);
          motor_run(0,0);
        
        }
        
      }
      break;
      
      case DOWN_SWIPE: {
        
        if(pressed_buttons[3-1] == false && pressed_buttons[4-1] == false) {
      
          Serial << "down swipe!" << endl;
          
          playTone(350, 70);
          delay(10);
          playTone(370, 70);
          
          motor_run(2, motor_speed);
          delay(50);
          motor_run(0,0);
        
        }
        
      }
      break;
      
      case TAP_SOUTH: {
        
        if(pressed_buttons[3-1] == false && pressed_buttons[4-1] == false) {
      
          Serial << "tap south!" << endl;
          
          playTone(350, 70);
          delay(10);
          playTone(370, 70);
          
          motor_run(2, motor_speed);
          delay(50);
          motor_run(0,0);
        
        }
        
      }
      break;
      
      case TAP_WEST: {
        Serial << "tap west!" << endl;
        
        playTone(250, 140);
        
        eeCutting(true);
        eeSetKnifePos(eePos-10);
        
      }
      break;
      
      case TAP_EAST: {
        Serial << "tap east!" << endl;
        
        playTone(200, 140);
        
        eeCutting(true);
        eeSetKnifePos(eePos+10);
        
      }
      break;
      
      case TAP_NORTH: {
        
        if(pressed_buttons[2-1] == false && pressed_buttons[1-1] == false) {
      
          Serial << "tap north!" << endl;
          
          playTone(300, 70);
          delay(10);
          playTone(320, 70);
          
          motor_run(1, motor_speed);
          delay(700);
          motor_run(0,0);
        
        }
        
      }
      break;
      
      case TAP_CENTER: {
        Serial << "tap center!" << endl;
        
      }
      break;
      
    }
    
    hover.setRelease(ts);
    
  }/* else if(hover.getStatus(ts) == 1) {
    
    Serial << "Hover's status is 1, resetting..." << endl;
    
    hover.begin(ts, reset);
    delay(100);
    
  }*/
  
}
