void ble_connected_routine() {
  

}


void ble_received_action(char action, char cmd, uint8_t key, uint16_t val, char delim) {
  Serial << "ble received action" << endl;
  Serial << action << "_" << cmd << "_" << key << "_" << val << "_" << delim << endl;
  
  if(action == '@') {
    
    switch(cmd) {
      case 'A':
        if(key == 1) {
          ACTION_STATE = 1;
        } else if(key == 2) {
          ACTION_STATE = 2;
        }
      break;
      case 'B':
        if(key == 1) {
          ACTION_STATE = 3;
        } else if(key == 2) {
          ACTION_STATE = 4;
        }
      break;
      case 'C':
        if(key == 1) {
          ACTION_STATE = 5;
          given_on_time = val;
        } else if(key == 2) {
          ACTION_STATE = 6;
          given_on_time = val;
        }
      break;
      case 'D':
        if(key == 1) {
          ACTION_STATE = 0;
        } else if(key == 2) {
          ACTION_STATE = 7;
        }
      break;
      case 'M':
        if(key == 1) {
          motorSpeedSlider(val);
        }
      break;
      case 'N':
        if(key == 1) {
          enc1Thresh(val);
        } else if(key == 2) {
          enc2Thresh(val);
        }
      break;
      case 'L':
        if(key == 1) {
          ledButton(val);
        }
      break;
      case 'R':
        if(key == 1) {
          routine_num = 0;
          running_routine = true;
        }
      break;
    }
    
  }
  
}


void go_parse_message() {
  
  Serial << "RX: ";
  
  for(int i=0; i<msg_len; i++) {
    Serial << msg_payload[i];
    promulgate.organize_message(msg_payload[i]);
  }
  
  Serial << endl;
  
}


void onTXCommandComplete() {
  Serial << "TX complete" << endl;
}


void ble_send_message(char action, char cmd, uint8_t key, uint16_t val, char delim) {
 
  //String s = String(action + cmd + key + "," + val + delim);
  
  //String s = String(String(action) + "" + String(cmd));// + String(key));// + "," + String(val) + String(delim));
  
  /*
  String s = "";
  s += action;
  s += cmd;
  s += String(key);
  s += ",";
  s += String(val);
  s += delim;
  */
  
  int sizee = 5;
  
  if(key >= 100) {
    sizee += 3;
  } else if(key >= 10) {
    sizee += 2;
  } else {
    sizee += 1;
  }
  
  if(val >= 1000) {
    sizee += 4;
  } else if(val >= 100) {
    sizee += 3;
  } else if(val >= 10) {
    sizee += 2;
  } else {
    sizee += 1;
  }
  
  
  char s[sizee]; // you have to be aware of how long your data can be
                 // not forgetting unprintable and null term chars
  sprintf(s,"%c%c%i,%i%c", action, cmd, key, val, delim);
  
  Serial << "the string: " << s << endl;
  
  /*
  uint8_t sendbuffer[20];
  s.getBytes(sendbuffer, 20);
  char sendbuffersize = min(20, s.length());
  */
  
  char sendbuffersize = sizee;
  
  Serial.print(F("\n* Sending -> \"")); Serial.print((char *)s); Serial.println("\"");

  BTLEserial.write((uint8_t *)s, sendbuffersize);
  
}

