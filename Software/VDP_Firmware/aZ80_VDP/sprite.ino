//================== Sprites =========================

// The sprites are max. 24x24 pixels, and are stored one byte per pixel
// representing the color index from the 256 color palette and a one
// bit per pixel mask:  -> 648 bytes data+mask * 48 sprites total -> 32kB
// 8-bit image with a 1-bit mask (set bits = opaque, unset bits = clear) 

void sprite_enable(uint8_t num, bool enable) {
  Sprite[num].enabled=enable;
}

void sprite_move(uint8_t num, int16_t posx, int16_t posy) {
  Sprite[num].xpos=posx;
  Sprite[num].ypos=posy;
}

void sprite_velocity(uint8_t num, int8_t velx, int8_t vely) {
  Sprite[num].xvel=velx;
  Sprite[num].yvel=vely; 
}

// Setup
void sprite_setup() {
  // Clear Sprites to solid color
  for (int i = 0; i < MAX_SPRITE_NUM; i++) {
    Sprite[i].xpos=0;
    Sprite[i].ypos=0;
    Sprite[i].xsize=16;
    Sprite[i].ysize=16;
    Sprite[i].enabled=false;
    memset(Sprite[i].data, 45+i*2, 576);    //  0x00 = Black
    memset(Sprite[i].mask, 0xff, 72);       //  0xff = Solid

    // DEMO!
    memcpy(Sprite[i].data, &BM_Logo, 256);
    memcpy(Sprite[i].mask, &BM_Logo_mask, 32);
    
    Sprite[i].xpos=10 + random(display.width() - 20);
    Sprite[i].ypos=10 + random(display.height() - 20);
    Sprite[i].xvel=2  - random(5);
    Sprite[i].yvel=2  - random(5);
    //Sprite[i].enabled=true; 
    
  }
}

// Update Sprites
void sprite_update() {
  // Update sprite movement
  for (int i=0; i<MAX_SPRITE_NUM; i++) {
    // After drawing each one, update positions, bounce off edges.
    Sprite[i].xpos += Sprite[i].xvel;
    if ( (Sprite[i].xpos <= 0) || (Sprite[i].xpos >= display.width()) ) Sprite[i].xvel *= -1;
    Sprite[i].ypos += Sprite[i].yvel;
    if ( (Sprite[i].ypos <= 0) || (Sprite[i].ypos >= display.height())) Sprite[i].yvel *= -1;
  }
}
