//
//  Display relevant routines.
//

// How this works:
//  We redraw all the graphics (Text, Sprites, Cursor, Mouse...) every frame into one of the dual screen buffers we have.
//  This displays a finished screen, while we draw a new screen in a second buffer and switch these buffers after are done.
//  This avoids screen flicker, saving of backgrounds and other nastyness. Thankfully the Pi Pico is fast enough for this.
//  See display_update() for layering order  (text in background, then graphics, sprites, menu and mouse on top)

/*   Code follows here   */

// Helper Functions
void display_tb_clear() {
  // clear textbuffer
  for (int i = 0; i < TEXTBUFSZ; i++) {
    text_buf[i]=32;
    text_fg[i]=fg_color;
    text_bg[i]=bg_color;
    text_attr[i]=0;
  }
}

void display_tb_clear_from(uint8_t x, uint8_t y) {
  // clear textbuffer
  for (int i = TEXTSX*y + x; i < TEXTBUFSZ; i++) {
    text_buf[i]=32;
    text_fg[i]=fg_color;
    text_bg[i]=bg_color;
    text_attr[i]=0;
  }
}

void display_tb_clear_to(uint8_t x, uint8_t y) {
  // clear textbuffer
  for (int i = 0; i < (TEXTSX*y + x); i++) {
    text_buf[i]=32;
    text_fg[i]=fg_color;
    text_bg[i]=bg_color;
    text_attr[i]=0;
  }
}

void display_tb_clear_line(uint8_t x, uint8_t y) {
  // clear textbuffer
  for (int i = TEXTSX*y; i < (TEXTSX*y + TEXTSX); i++) {
    text_buf[i]=32;
    text_fg[i]=fg_color;
    text_bg[i]=bg_color;
    text_attr[i]=0;
  }
}

void display_tb_clear_line_from(uint8_t x, uint8_t y) {
  // clear textbuffer
  for (int i = TEXTSX*y + x; i < (TEXTSX*y + TEXTSX); i++) {
    text_buf[i]=32;
    text_fg[i]=fg_color;
    text_bg[i]=bg_color;
    text_attr[i]=0;
  }
}

void display_tb_clear_line_to(uint8_t x, uint8_t y) {
  // clear textbuffer
  for (int i = TEXTSX*y; i < (TEXTSX*y + x); i++) {
    text_buf[i]=32;
    text_fg[i]=fg_color;
    text_bg[i]=bg_color;
    text_attr[i]=0;
  }
}

void display_tb_scroll() {
  // scroll textbuffer
  for (int i = TEXTSX; i < TEXTBUFSZ; i++) {
    text_buf[i-TEXTSX]  = text_buf[i];
    text_fg[i-TEXTSX]   = text_fg[i];
    text_bg[i-TEXTSX]   = text_bg[i];
    text_attr[i-TEXTSX] = text_attr[i];
  }  
  for (int i = TEXTBUFSZ - TEXTSX; i < TEXTBUFSZ; i++) {
    text_buf[i]=32;
    text_fg[i]=fg_color;
    text_bg[i]=bg_color;
    text_attr[i]=0;
  }
}

void display_pal_setcolor(uint8_t col, uint16_t rgb) {
   uint16_t* palette = display.getPalette();
   palette[col] = rgb;
}

void display_drawChar(int x0, int y0, uint8_t c, uint8_t fgcol, uint8_t bgcol, uint8_t attr) {
  // draw character onto display buffer
  if (c>31) {
    for (int j = 0; j < 8; j++) {
      uint8_t font_bits = BM_font8x8[(c - 32) * 8 + j ];
      // TODO: More Attributes then just Blink or Reverse
      if ( attr & TEXTATTR_REVERSE ) { 
        font_bits = ~font_bits;
      }

      if ( attr & TEXTATTR_BLINK ) {
        if ( (millis() / 500) & 1 ) {
          for (int i = 0; i < 8; i++) {
            if (screen_mode > 0 && bgcol == 0) {
              // Font has transparent background with color 0
              if (font_bits & (1u << (7-i) ) ) display.drawPixel(x0+i, y0+j, fgcol);
            } else {
              // Font is drawn solid with background color
              display.drawPixel(x0+i, y0+j, font_bits & (1u << (7-i) ) ? fgcol : bgcol);
            }
          }
        }
      } else {
          for (int i = 0; i < 8; i++) {
            if (screen_mode > 0 && bgcol == 0) {
              // Font has transparent background with color 0
              if (font_bits & (1u << (7-i) ) ) display.drawPixel(x0+i, y0+j, fgcol);
            } else {
              // Font is drawn solid with background color
              display.drawPixel(x0+i, y0+j, font_bits & (1u << (7-i) ) ? fgcol : bgcol);
            }
          }
      }
    }
  }
}

// OVERLAY: Status/Config Screen (Toggled by GUI/Win + ESC)
void display_drawStatus() {
      int freemem = rp2040.getFreeHeap();
      int gpu_ticks = ( ( rp2040.getCycleCount64() - cpu_ticks ) / 1024 );
      int gpu_hz = ( 252*1024 / gpu_ticks ); 
      
      display.fillRect(36,30,250,181,7);
      display.drawRect(37,31,248,179,0);
      display.setTextColor(0);
      display.setCursor(110,38);
      display.print("aZ80 VDP Status");
      display.setCursor(48,54);
      display.printf("Serial: %i 8N1", SER1_SPEED);
      #ifdef DEBUG
        display.setCursor(48,64);
        display.printf("Debug: %i 8N1", SER2_SPEED);
      #endif      

      // Status GUI: Display USB Devices
      int i=268;
      if ( is_wifi_capable ) {
        display.drawBitmap(i,36, BM_Wifi,     8, 8, 0);
        i-=9;
      }
      if ( is_filesystem_mounted ) {
        display.drawBitmap(i,36, BM_Disk,     8, 8, 0);
        i-=9;
      }
      if ( is_joystick_mounted ) {
        display.drawBitmap(i,36, BM_Joystick, 8, 8, 0);
        i-=9;
      }
      if ( is_mouse_mounted ) {
        display.drawBitmap(i,36, BM_Mouse,    8, 8, 0);
        i-=9;
      }
      if ( is_keyboard_mounted ) {
        display.drawBitmap(i,36, BM_Keyboard, 8, 8, 0);
      }

      // Status GUI: Display Cursor/Mouse Position
      display.setCursor(160,54);
      display.printf("Cursor X: %02d  Y: %02d",cursorx,cursory);
      display.setCursor(160,64);
      display.printf("Mouse  X: %03d Y: %03d",mousex,mousey);
      
      // Status GUI: Display Charset
      display.setCursor(62,80);
      display.print("Charset (32-255)");
      for (int i=0; i<12; i++) {
        display.setCursor(42,80+i*8);
        display.printf("%3i", 20+i*20);
      }
      for (int i=0; i<20; i++) {
        display_drawChar(62+i*8, 80,i+20, 0,24,0);
        display_drawChar(62+i*8, 88,i+40, 0,24,0);
        display_drawChar(62+i*8, 96,i+60, 0,24,0);
        display_drawChar(62+i*8,104,i+80, 0,24,0);
        display_drawChar(62+i*8,112,i+100,0,24,0);
        display_drawChar(62+i*8,120,i+120,0,24,0);
        display_drawChar(62+i*8,128,i+140,0,24,0);
        display_drawChar(62+i*8,136,i+160,0,24,0);
        display_drawChar(62+i*8,144,i+180,0,24,0);
        display_drawChar(62+i*8,152,i+200,0,24,0);
        display_drawChar(62+i*8,160,i+220,0,24,0);
        if ((i+240) < 256) display_drawChar(62+i*8,168,i+240,0,24,0);
      }

      // Status GUI: Display GFX Infos
      display.setCursor(227,80);
      display.print("GFX Infos");
      display.setCursor(227,88);
      display.printf("Mode: %i",screen_mode);
      display.setCursor(227,96);
      display.printf("X:    %03i",gfx_curx);
      display.setCursor(227,104);
      display.printf("Y:    %03i",gfx_cury);
      
      // Status GUI: Display Sprite Buffers
      uint8_t spidx = int(sprite_gui/16);
      display.setCursor(227,130);
      display.printf("Sprite %02i",spidx);
      display.setCursor(227,169);
      display.print("Data Mask");
      display.drawRect(225,140,28,28,0);
      display.drawRect(254,140,28,28,0);
      // Draw Sprite and increment to next sprite
      display.drawGrayscaleBitmap(227,142,Sprite[spidx].data, Sprite[spidx].mask, Sprite[spidx].xsize, Sprite[spidx].ysize);
      display.drawBitmap(256,142,Sprite[spidx].mask, Sprite[spidx].xsize, Sprite[spidx].ysize,0);
      sprite_gui++;
      if (sprite_gui>=MAX_SPRITE_NUM*16) sprite_gui=0;

      // Status GUI: Display Color Palette 
      display.setCursor(42,186);
      display.printf("Palette");
      for (int i=0; i<64; i++) {
        display.fillRect(87+i*3,181,3,3,i);    
        display.fillRect(87+i*3,185,3,3,i+64);    
        display.fillRect(87+i*3,189,3,3,i+128);    
        display.fillRect(87+i*3,193,3,3,i+192);    
      }  

      // Status GUI: Display Timing Infos
      display.setCursor(42,200);
      display.printf("GPU Ticks: %i %ifps", gpu_ticks, gpu_hz);
      display.setCursor(178,200);
      display.printf("Free Mem:%6ikB",freemem);
}

void display_reset() {
  // clear textbuffer and setup cursor/mouse
  display_tb_clear();

  // Initialize color palette from table in sprites.h. Rather than
  // calling setColor() for each one, we can just dump it directly...
  memcpy(display.getPalette(), BM_Palette, sizeof BM_Palette);
  display.swap(false, true); // Duplicate same palette into front & back buffers

  screen_bg_color = 0;
  fg_color     = 7;
  bg_color     = 0;
  current_attr = 0;
  cycle_speed  = 3;
  cycle_count  = 0;
  status_gui_on = false;
  cursorx = 0;
  cursory = 0;
  cursor_mode = CURSOR_TYPE_BLOCK_BLINK;
  mouse_pointer = MOUSE_TYPE_OFF;

  // Initialize sprites
  sprite_setup();

  // Initialize GFX layer
  gfx_setup();
}


//--------------------------------------------------------------------+
// Setup and Loop 
//--------------------------------------------------------------------+

void display_setup() {
  // check for insufficient RAM, if so halt and blink
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 100) & 1);
  }

  // reset display to default
  display_reset();
  // status_gui_on = true;
}

void display_update() {
    // This is the main display routine and updates the full screen
    // in the Layer order defined below. This is done every frame
    // with about 60fps with dual buffer enabled. Dual buffer 
    // eliminates all tearing and redraw issues.
    uint16_t* palette = display.getPalette();
    cpu_ticks = rp2040.getCycleCount64();

    // === LAYER 0 - GFX Layer  ===
    if (screen_mode == 0) {
      // mode 0 has blank background
      display.fillScreen(screen_bg_color);
    } else {
      // mode 1 & 2 have gfx buffer as background
      memcpy(display.getBuffer(), gfx_buffer.getBuffer(), DISPLAYSX * DISPLAYSY);
    }

    // === LAYER 1 - TEXT / TILE (Background) ===
    // Draw display text buffer with fg/bg color
    for (int yp = 0; yp < TEXTSY; yp++) {
      for (int xp = 0; xp < TEXTSX; xp++) {
        int i = xp + yp*TEXTSX ;
        display_drawChar(xp*8,yp*8,text_buf[i],text_fg[i],text_bg[i],text_attr[i]);
      }
    }

    // Draw text cursor
    if ( cursor_mode > CURSOR_TYPE_OFF ) {
      int i=cursorx+cursory*TEXTSX;
      // Blinking Cursors
      if ((millis() / 500) & 1) {
        if (cursor_mode == CURSOR_TYPE_BLOCK_BLINK) display.fillRect(cursorx*8,cursory*8,8,8,text_fg[i]);
        if (cursor_mode == CURSOR_TYPE_LINE_BLINK)  display.drawLine(cursorx*8,cursory*8+8,cursorx*8+8,cursory*8+8,text_fg[i]);
        if (cursor_mode == CURSOR_TYPE_BAR_BLINK)   display.drawLine(cursorx*8,cursory*8,cursorx*8,cursory*8+8,text_fg[i]);
      }  
      if (cursor_mode == CURSOR_TYPE_BLOCK_STEADY) display.fillRect(cursorx*8,cursory*8,8,8,text_fg[i]);
      if (cursor_mode == CURSOR_TYPE_LINE_STEADY)  display.drawLine(cursorx*8,cursory*8+8,cursorx*8+8,cursory*8+8,text_fg[i]);
      if (cursor_mode == CURSOR_TYPE_BAR_STEADY)   display.drawLine(cursorx*8,cursory*8,cursorx*8,cursory*8+8,text_fg[i]);
    }

    // === LAYER 2 - SPRITE LAYER ===
    // loop through all sprites and if sprite is enabled, draw it
    for (int i=0; i<MAX_SPRITE_NUM; i++) {
      if ( Sprite[i].enabled ) {
        display.drawGrayscaleBitmap(Sprite[i].xpos,Sprite[i].ypos,Sprite[i].data, Sprite[i].mask, Sprite[i].xsize, Sprite[i].ysize);
      }
    }

    // === LAYER 3 - Status/Config Screen (Toggled by GUI/Win/Cmd Key + ESC Key) ===
    if (status_gui_on) {
      display_drawStatus();
    }
    
    // === LAYER 4 - MOUSE CURSOR (top most layer) ===
    // currently very simple ones
    if ( mouse_pointer == MOUSE_TYPE_POINT) {
      display.drawPixel(mousex,mousey,255);
    }
    if ( mouse_pointer == MOUSE_TYPE_CROSS) {
      display.drawLine(mousex,mousey-4,mousex,mousey+4,255);
      display.drawLine(mousex-4,mousey,mousex+4,mousey,255);
    }

    // === Housekeeping follows here ===

    // Automated Color Cycling of the last 8 palette entries
    if (cycle_count > cycle_speed ) {
        uint16_t  temp = palette[255];
        palette[255] = palette[254];           
        palette[254] = palette[253];           
        palette[253] = palette[252];           
        palette[252] = palette[251];           
        palette[251] = palette[250];
        palette[250] = palette[249];
        palette[249] = palette[248];
        palette[248] = temp;
        cycle_count = 0;           
    }
    cycle_count++;

    // Swap front/back buffers, do not duplicate current screen state to next frame,
    display.swap(false, true);
}   
