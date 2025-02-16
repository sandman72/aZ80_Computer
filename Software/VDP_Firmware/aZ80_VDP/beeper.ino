// Code for basic Bell sound. Not more so far.
// 

void beeper_setup() {
  pinMode(BEEPER_PIN, OUTPUT);
  digitalWrite(BEEPER_PIN, LOW);   
}

void beeper_update() {
  if (beeper_on) {
    digitalWrite(BEEPER_PIN, HIGH);   
    beeper_timer = BEEPER_LOOPS;
    DEBUG_PRINT("Beep!\n\r");
    beeper_on = false;
  }
  if (beeper_timer > 0) {
    beeper_timer--;
    if (beeper_timer == 0) {
      digitalWrite(BEEPER_PIN, LOW);       
    }
  }
}
