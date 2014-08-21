void state_machine() {
 
  // this is for states that have no real way of exiting,
  // other than a different action being called.
  // so we check for this, and tell that state to stop.
  if(ACTION_STATE != PREV_ACTION_STATE && PREV_ACTION_STATE == 7) {
    int tempy = ACTION_STATE;
    ACTION_STATE = PREV_ACTION_STATE;
    state_machine_stop();
    ACTION_STATE = tempy;
  }
  
  switch(ACTION_STATE) {
    case 0:
      if(ACTION_STATE != PREV_ACTION_STATE) {
        // release motors
        motor_run(0, 0);
      }
    break;
    case 1:
      if(ACTION_STATE != PREV_ACTION_STATE) {
        motor_direction_up = true;
      }
    
      goToTop();
    break;
    case 2:
      if(ACTION_STATE != PREV_ACTION_STATE) {
        motor_direction_up = false;
      }
    
      goToBottom();
    break;
    case 3:
      if(ACTION_STATE != PREV_ACTION_STATE) {
        motor_direction_up = true;
        ticks_go(1, true);
      }
      
      stepUp();
    break;
    case 4:
      if(ACTION_STATE != PREV_ACTION_STATE) {
        motor_direction_up = false;
        ticks_go(1, false);
      }
      
      stepDown();
    break;
    case 5:
      if(ACTION_STATE != PREV_ACTION_STATE) {
        motor_direction_up = true;
        startActionTime = millis();
        Serial << "start time: " << startActionTime << endl;
      }
      
      upTime();
    break;
    case 6:
      if(ACTION_STATE != PREV_ACTION_STATE) {
        motor_direction_up = false;
        startActionTime = millis();
      }
      
      downTime();
    break;
    case 7:
      if(ACTION_STATE != PREV_ACTION_STATE) {
        tick_holding = true;
      }
      
      holdButton();
    break;
  }
  
  
  if(ACTION_STATE != PREV_ACTION_STATE) {
    PREV_ACTION_STATE = ACTION_STATE;
  }
  
}





void state_machine_stop() {
  
  if(running_routine) {
    
    motor_run(0, 0);
    
    //delay(1000);
    
    if(frame_num == 3) {
      frame_num--;
    } else {
      frame_num++;
    }
    
    framefirstpass = true;
    
  } else {
    
    switch(ACTION_STATE) {
      case 0:
        
      break;
      case 1:
        motor_direction_up = false;
        ACTION_STATE = 0;
      break;
      case 2:
        motor_direction_up = true;
        ACTION_STATE = 0;
      break;
      case 3:
        ACTION_STATE = 0;
      break;
      case 4:
        ACTION_STATE = 0;
      break;
      case 5:
        ACTION_STATE = 0;
        motor_run(0, 0);
      break;
      case 6:
        ACTION_STATE = 0;
        motor_run(0, 0);
      break;
      case 7:
        tick_holding = false;
        ACTION_STATE = 0;
      break;
    }
  
  }
  
}








