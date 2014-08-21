
void ble_check_state() {
  
  uint16_t slice = millis() % 1000;
  
  // Ask what is our current status
  aci_evt_opcode_t status = BTLEserial.getState();
  // If the status changed....
  if (status != laststatus) {
    // print it out!
    if (status == ACI_EVT_DEVICE_STARTED) {
        Serial.println(F("* Advertising started"));
        digitalWrite(status_led, HIGH);
    }
    if (status == ACI_EVT_CONNECTED) {
        Serial.println(F("* Connected!"));
        initialLoad();
    }
    if (status == ACI_EVT_DISCONNECTED) {
        Serial.println(F("* Disconnected or advertising timed out"));
        digitalWrite(status_led, LOW);
    }
    // OK set the last status change to this one
    laststatus = status;
  }

  if (status == ACI_EVT_CONNECTED) {
    
    digitalWrite(status_led, slice < 100 || (slice > 200 && slice < 300));  
    
    uint8_t i = 0;
  
    while (BTLEserial.available()) {
      char c = BTLEserial.read();
      //Serial << c << endl;
      if(i<20) msg_payload[i] = c;
      msg_len = i;
      i++;
    }
    
    if(i<20) msg_payload[i] = ' ';
    msg_len = i;
    i++;
    
    if(i>1) go_parse_message();
    
    ble_connected_routine();
    
  }
  
}


