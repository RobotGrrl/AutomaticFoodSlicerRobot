void runStateMachineTest() {
 
  int num_steps = 11;
  
  if(current_step > num_steps-1) return;
  
  if(millis()-last_step > 10000) {
    stateMachineTest(++current_step);
    last_step = millis();
  }
  
}



void stateMachineTest(int s) {

  Serial << s << ". running" << endl;
  
  switch(s) {
  
    case 1: {
      
      // tells knife to move to 90 degree position
      // result: knife should not move knife in ALL_OFF state
      char str[7] = {'@', 'K', '1', ',', '9', '0', ';'};
      sendTest(str, 7);
      
    }
    break;
    
    case 2: {
  
      // sets state to MOVING
      // result: led should be blinking on/off every 1000ms
      char str[6] = {'@', 'S', '1', ',', '0', ';'};
      sendTest(str, 6);
      
    }
    break;
  
    case 3: {
    
      // tells knife to move to 90 degree position
      // result: knife should move as it is in MOVING state
      char str[7] = {'@', 'K', '1', ',', '9', '0', ';'};
      sendTest(str, 7);  
      
    }
    break;
    
    case 4: {
    
      // sets state to CUTTING
      // result: led should be blinking on/off every 100ms
      char str[6] = {'@', 'S', '2', ',', '0', ';'};
      sendTest(str, 6);
      
    }
    break;
    
    case 5: {
    
      // tells knife to move to 90 degree position
      // result: knife should move as it is in CUTTING state
      char str[7] = {'@', 'K', '1', ',', '9', '0', ';'};
      sendTest(str, 7);  
      
    }
    break;
    
    case 6: {
    
      // sets state to ALL_OFF
      // result: led should be off
      char str[6] = {'@', 'S', '0', ',', '0', ';'};
      sendTest(str, 6);
      
    }
    break;
    
    case 7: {
    
      // tells knife to move to 90 degree position
      // result: knife should not move as it is in ALL_OFF state
      char str[7] = {'@', 'K', '1', ',', '9', '0', ';'};
      sendTest(str, 7);  
      
    }
    break;
    
    case 8: {
    
      // sets state to CUTTING
      // result: led should be blinking on/off every 1000ms
      char str[6] = {'@', 'S', '1', ',', '0', ';'};
      sendTest(str, 6);
      
    }
    break;
    
    case 9: {
    
      // tells knife to move to 90 degree position
      // result: knife should move as it is in CUTTING state
      char str[7] = {'@', 'K', '1', ',', '9', '0', ';'};
      sendTest(str, 7);  
      
    }
    break;
    
    case 10: {
    
      // sets state to an unknown state
      // result: should default to ALL_OFF state
      char str[6] = {'@', 'S', '5', ',', '0', ';'};
      sendTest(str, 6);
      
    }
    break;
  
    case 11: {
    
      // tells knife to move to 90 degree position
      // result: knife should not move as it is in ALL_OFF state
      char str[7] = {'@', 'K', '1', ',', '9', '0', ';'};
      sendTest(str, 7); 
      
    }
    break;
  
  }
  
}


void sendTest(char *str, int len) {
 
  Serial << "sending test..." << endl;
  for(int i=0; i<7; i++) {
    char c = str[i];
    promulgate.organize_message(c);
  }
  
}

