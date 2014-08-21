void ee_received_action(char action, char cmd, uint8_t key, uint16_t val, char delim) {

  if(action == '@') {
    
    if(cmd == 'H' && key == 1) {
      Serial << "the green knife end effector!" << endl;
      end_effector_type = 0;
    }
    
  }

  
}

void ee_transmit_complete() {
  last_ee_cmd = millis();
}

