//
// Serial routines
//
// Serial1  = RX GPIO  1 /  TX GPIO  2  -> aZ80 Bus
// Serial2  = RX GPIO 21 /  TX GPIO 20  -> Debug Header
//

void serial_setup() {
  //Serial1.setPollingMode(true);
  Serial1.setFIFOSize(256);
  Serial1.begin(SER1_SPEED);  // Z80 does 57.6kbaud

  #ifdef DEBUG
     Serial2.setTX(20);
     Serial2.setRX(21);
     Serial2.begin(SER2_SPEED); // Debug port
  #endif
}

void serial_update() {
  uint8_t receivedChar=0;
  //String receivedChar;
  while (Serial1.available() > 0) {
    receivedChar = Serial1.read();
    // DEBUG_PRINT("%c", receivedChar);
    handle_new_character(receivedChar);
  }
}
