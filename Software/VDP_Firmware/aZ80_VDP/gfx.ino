//================== GFX =========================

// Todo: GFX command list implementation. 
//
// Idea: implement a list of simple GFX commands sent via serial 
// that draw different shapes or objects from aZ80 like:
// - ESC[1;0;0g        Move GFX cursor to absolute position x=0 y=0
// - ESC[2;10;10g      Move GFX cursor to relative position x+10 y+10
// - ESC[4;10;20;42g   Draw Pixel at position x=10 y=20 with color 42
// - ESC[6;80;120;37g  Draw Line to absolute position x=80 Y=120 with color 37
// - ESC[7;80;120;37g  Draw Line to relative position x+80 y+120 with color 37
// - ESC[10;80;90;17g  Draw Rectangle at cursor with width 80, height 90 with color 17
// - ESC[11;80;90;17g  Draw filled Rectangle at cursor with width 80, height 90 with color 17
// - ESC[20;40;64g     Draw Circle at cursor with radius 40 with color 64
// - ESC[21;40;64g     Draw filled Circle at cursor with radius 40 with color 64
// - ESC[99g           Clear GFX buffer and set cursor to x=0 y=0
// ...
//
// ESC sequence commands are stored in GFXlist data array in ansi_esc.ino tab
// actual drawing is done in display.ino tab
//
// end of list if draw command is equal to zero

void gfx_draw(uint8_t cmd, uint16_t x, uint16_t y, uint8_t col) {
  DEBUG_PRINT("GFX %i %i %i %i\r\n",cmd,x,y,col);
  switch(cmd){
    case 1:
      gfx_curx = x; 
      gfx_cury = y;
      break;

    case 2: 
      gfx_curx = gfx_curx + x; 
      gfx_cury = gfx_cury + y;
      break;

    case 4: 
      gfx_buffer.drawPixel(x,y,col);
      break;

    case 6: 
      gfx_buffer.drawLine(gfx_curx,gfx_cury,x,y,col);
      gfx_curx = x; 
      gfx_cury = y;
      break;

    case 7: 
      gfx_buffer.drawLine(gfx_curx,gfx_cury,gfx_curx+x,gfx_cury+y,col);
      gfx_curx = gfx_curx + x; 
      gfx_cury = gfx_cury + y;
      break;

    case 10: 
      gfx_buffer.drawRect(gfx_curx,gfx_cury,gfx_curx+x,gfx_cury+y,col);
      gfx_curx = gfx_curx + x; 
      gfx_cury = gfx_cury + y;
      break;

    case 11: 
      gfx_buffer.fillRect(gfx_curx,gfx_cury,gfx_curx+x,gfx_cury+y,col);
      gfx_curx = gfx_curx + x; 
      gfx_cury = gfx_cury + y;
      break;

    case 20: 
      gfx_buffer.drawCircle(gfx_curx,gfx_cury,x,y);
      break;

    case 21: 
      gfx_buffer.fillCircle(gfx_curx,gfx_cury,x,y);
      break;

    // Clear Screen
    case 99: 
      gfx_buffer.fillScreen(screen_bg_color);
      gfx_curx = 0; 
      gfx_cury = 0;
      break;

    // Default do nothing
    default:
      break;
  }
}

void gfx_reset() {
  gfx_buffer.fillScreen(screen_bg_color);
  gfx_curx = 0;
  gfx_cury = 0;
}

// Setup
void gfx_setup() {
  gfx_reset();
}
