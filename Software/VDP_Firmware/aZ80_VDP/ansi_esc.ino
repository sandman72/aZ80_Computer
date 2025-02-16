//
// This holds the ANSI, ASCII and ESC helpers
//

// We support the following ESC Sequences
//
// Single Chars:
// ESC[7               | Save the cursor position
// ESC[8               | Move cursor to previously saved position
// ESC[c               | Reset Terminal
// ESC[s               | Save the cursor position
// ESC[u               | Move cursor to previously saved position
// ESC[A               | Move the cursor up
// ESC[B               | Move the cursor down
// ESC[C               | Move the cursor forward 
// ESC[D               | Move the cursor backward
// ESC[H               | Move to 0-0
// ESC[J               | Clear screen from cursor
// ESC[K               | Clear line from cursor
//
// Multiple chars:
// ESC[?12l            | Cursor not blinking 
// ESC[?12h            | Cursor blinking
// ESC[?25l            | Cursor invisible
// ESC[?25h            | Cursor visible
// ESC[0m              | Text Normal
// ESC[5m              | Text Blink on
// ESC[7m              | Text Reverse on
// ESC[30m             | Set foreground color to palette entry 0 (usually black)
//  to
// ESC[37m             | Set foreground color to palette entry 7 (usually white)
// ESC[38;5;-n-m       | Set foreground color to palette entry -n- (0-255)
// ESC[40m             | Set background color to palette entry 0 (usually black)
//  to
// ESC[47m             | Set background color to palette entry 7 (usually white)
// ESC[48;5;-n-m       | Set background color to palette entry -n- (0-255)
// ESC[49;5;-n-m       | Set screen background color to palette entry -n- (usually black)
// ESC[50;c;r;g;bm     | Set palette entry -c- to RGB color -r-,-g-,-b- (0-255)
// ESC[5n              | Return terminal status "OK"
// ESC[6n              | Return cursor position    -> returns ESC[-n-;-m-R   -n- = y   -m- = x     (reversed)
// ESC[9n              | Return mouse pos/buttons  -> returns ESC[-n-;-m-;-b-R   -n- = x   -m- = y  -b- = buttons 
// ESC[-n-A            | Move the cursor up -n- lines
// ESC[-n-B            | Move the cursor down -n- lines
// ESC[-n-C            | Move the cursor forward -n- characters
// ESC[-n-D            | Move the cursor backward -n- characters
// ESC[-Row-;-Col-H    | Move to -Row-,-Col-
// ESC[0J              | clear screen from cursor
// ESC[1J              | clear screen to cursor
// ESC[2J              | Clear the screen and move the cursor to 0-0
// ESC[3J              | same as ESC[2J
// ESC[0K              | Clear from cursor to the end of the line
// ESC[1K              | Clear from the beginning of the current line to the cursor
// ESC[2K              | Clear the whole line
// ESC[nS              | scroll whole page up by n rows (default 1 if n missing)

// Sprites
// ESC[80;n;em         | Set sprite state -e- enable=1 disable=0 
// ESC[81;n;x;ym       | Move sprite -n- to position -x- -y- 
// ESC[82;n;x;ym       | Set sprite velocity to -x- -y- 

// Graphics
// - ESC[1;0;0;g        Move GFX cursor to position x=0 y=0
// - ESC[5;10;20;42;g   Draw Pixel at position x=10 y=20 with color 42
// - ESC[10;80;90;17;g  Draw Rectangle at cursor with width 80, height 90 with color 17
// - ESC[11;80;90;17;g  Draw filled Rectangle at cursor with width 80, height 90 with color 17
// - ESC[20;40;64;g     Draw Circle at cursor with radius 40 with color 64
// - ESC[21;40;64;g     Draw filled Circle at cursor with radius 40 with color 64
// - ESC[99g            Clear command list and set cursor to x=0 y=0

// Cursor Modes
// ESC[0 q             | Cursor Off        
// ESC[1 q             | Blinking Block
// ESC[2 q             | Steady Block
// ESC[3 q             | Blinking Underline
// ESC[4 q             | Steady Underline
// ESC[5 q             | Blinking Bar
// ESC[6 q             | Steady Bar
#define CURSOR_TYPE_OFF 0
#define CURSOR_TYPE_BLOCK_BLINK 1
#define CURSOR_TYPE_BLOCK_STEADY 2
#define CURSOR_TYPE_LINE_BLINK 3
#define CURSOR_TYPE_LINE_STEADY 4
#define CURSOR_TYPE_BAR_BLINK 5
#define CURSOR_TYPE_BAR_STEADY 6

// Mouse Modes
// ESC[?1020h           | Return Mouse Position -> returns ESC[-n-;-m-;-b-R   -n- = x  -m- = y  -b- = buttons
// ESC[?1030h           | Mouse Off
// ESC[?1031h           | Mouse moves text cursor (TODO)
// ESC[?1032h           | Mouse shows as single point
// ESC[?1033h           | Mouse shows as cross hair
// ESC[?1034h           | Mouse shows as user defined sprite (TODO)
// ESC[?1030l           | Mouse Off
#define MOUSE_TYPE_OFF 0
#define MOUSE_TYPE_TEXT 1
#define MOUSE_TYPE_POINT 2
#define MOUSE_TYPE_CROSS 3
#define MOUSE_TYPE_SPRITE 4

// OS Commands (TODO)
// commands are send in form of ESC]command; with a closing
// -ESC]dir;        | Return directory listing of USB Stick
// -ESC]load -n-;   | Loads/Prints out file -n- to terminal
// -ESC]save -n-;   | Saves terminal output to file -n-
// -ESC]del -n-;    | Deletes file -n-

// Common ASCII Codes
#define ASCII_NULL      0x00        // null
#define ASCII_BELL      0x07        // bell
#define ASCII_BS        0x08        // backspace
#define ASCII_TAB       0x09        // horizontal tab
#define ASCII_LF        0x0A        // line feed
#define ASCII_VT        0x0B        // vertical tab
#define ASCII_FF        0x0C        // form feed (new page)
#define ASCII_CR        0x0D        // carriage return
#define ASCII_ESC       0x1B        // escape
#define ASCII_FS        0x1C        // file seperator
#define ASCII_GS        0x1D        // group seperator
#define ASCII_RS        0x1E        // row seperator
#define ASCII_US        0x1F        // unit seperator
#define ASCII_SPACE     0x20        // space
#define ASCII_HASH      0x23        // #
#define ASCII_LB        0x28        // (
#define ASCII_RB        0x29        // )
#define ASCII_COMMA     0x2C        // ,
#define ASCII_PERIOD    0x2E        // .
#define ASCII_SLASH     0x2f        // /
#define ASCII_SEMI      0x3B        // ;
#define ASCII_LSB       0x5B        // [
#define ASCII_BSLASH    0x5C        // back slash
#define ASCII_RSB       0x5D        // ]

// escape sequence state
#define ESC_READY               0
#define ESC_ESC_RECEIVED        1
#define ESC_PARAMETER_READY     2

#define MAX_ESC_PARAMS          5
#define MAX_ESC_STRING          128

// escape sequence related code is found in display.ino

static int esc_state = ESC_READY;
static int esc_parameters[MAX_ESC_PARAMS];
static char esc_os_str[MAX_ESC_STRING];
static bool parameter_q; // true when ? in request
static bool parameter_p; // true when parenthesis in request
static bool parameter_r; // true when reverse parenthesis in request
static bool parameter_sp; // true when ESC sequence contains a space
static int esc_parameter_count;
static char esc_c1;
static char esc_final_byte;

void ansi_setup(){
  reset_escape_sequence();
}

void reset_escape_sequence(){
    clear_escape_parameters();
    esc_state=ESC_READY;
    memset(esc_os_str, 0, sizeof(esc_os_str));
    esc_c1=0;
    esc_final_byte=0;
    parameter_q=false;
    parameter_p=false;
    parameter_r=false;
    parameter_sp=false;
}

void clear_escape_parameters(){
    for(int i=0;i<MAX_ESC_PARAMS;i++){
        esc_parameters[i]=0;
    }
    esc_parameter_count = 0;
}

//  ESC Control Related Stuff follows here

void esc_sequence_received(){
    /*
    // these should now be populated:
        static int esc_parameters[MAX_ESC_PARAMS];
        static int esc_parameter_count;
        static unsigned char esc_c1;
        static unsigned char esc_final_byte;
    */
  int n,m;
  if(esc_c1=='['){
    // ESC Command Sequence to control terminal - CSI
    // DEBUG_PRINT("ESC final byte:  %c  %i  0x%02x\n\r",esc_final_byte,esc_final_byte,esc_final_byte);
    switch(esc_final_byte){
        case 'H':
        case 'f':
            // Moves the cursor to row n, column m
            //Move cursor to upper left corner ESC [H
            //Move cursor to upper left corner ESC [;H
            //Move cursor to screen location v,h ESC [<v>;<h>H
            //Move cursor to upper left corner ESC [f
            //Move cursor to upper left corner ESC [;f
            //Move cursor to screen location v,h ESC [<v>;<h>f

            n = esc_parameters[0];
            m = esc_parameters[1];
            cursorx=m;
            cursory=n;
            break;

        case 'E': // ESC[#E moves cursor at beginning of next line, # lines down
            n = esc_parameters[0];
            if (n==0) n=1;
            cursorx=0;
            cursory=cursory+n;
            if (cursory >TEXTSY-1) { 
              cursory = TEXTSY-1;
            }
            break;

        case 'F':  // ESC[#F  moves cursor to beginning of previous line, # lines up
            n = esc_parameters[0];
            if (n==0) n=1;
            cursorx=0;
            cursory=cursory-n;
            if (cursory < 0) { 
              cursory = 0;
            }
            break;

        case 'd': // ESC[#d  moves cursor to an absolute # line
            n = esc_parameters[0];
            if (n==0) n=1;
            cursory=n;
            break;

        case 'G': // ESC[#G  moves cursor to column #
            n = esc_parameters[0];
            if (n==0) n=1;
            cursorx=n;
            break;

        case 'h':
            //[ ? 12 h    Text Cursor Enable Blinking
            //[ ? 25 h    Cursor ON
            //[ ? 50 h    Cursor ON
            //[ ? 1030 h  Mouse OFF
            //[ ? 1031 h  Mouse Text
            //[ ? 1032 h  Mouse Point
            //[ ? 1033 h  Mouse Crosshair
            //[ ? 1034 h  Mouse Sprite

            if(parameter_q){
                if(esc_parameters[0]==25 || esc_parameters[0]==50){
                    // show csr
                    cursor_mode = CURSOR_TYPE_BLOCK_BLINK;
                }
                else if(esc_parameters[0]==12){
                    //Text Cursor Enable Blinking
                    cursor_mode = CURSOR_TYPE_BLOCK_BLINK;
                }
                else if(esc_parameters[0]==1020){
                    // Return mouse cursor position (TODO)
                    DEBUG_PRINT("Mouse Position x: %i  y: %i  buttons: ",mousex,mousey);
                }
                else if(esc_parameters[0]==1030){
                    // Set mouse cursor type
                    mouse_pointer = MOUSE_TYPE_OFF;
                }
                else if(esc_parameters[0]==1031){
                    // Set mouse cursor type
                    mouse_pointer = MOUSE_TYPE_TEXT;
                }
                else if(esc_parameters[0]==1032){
                    // Set mouse cursor type
                    mouse_pointer = MOUSE_TYPE_POINT;
                }
                else if(esc_parameters[0]==1033){
                    // Set mouse cursor type
                    mouse_pointer = MOUSE_TYPE_CROSS;
                }
                else if(esc_parameters[0]==1034){
                    // Set mouse cursor type
                    mouse_pointer = MOUSE_TYPE_SPRITE;
                }
                
            }
            break; // case 'h'

        case 'l':
            //[ ? 12 l      Text Cursor Disable Blinking
            //[ ? 25 l      Cursor OFF
            //[ ? 50 l      Cursor OFF
            //[ ? 1030 l    Mouse OFF
            
            if(parameter_q){
                if(esc_parameters[0]==25 || esc_parameters[0]==50){
                    // hide csr
                    cursor_mode = CURSOR_TYPE_OFF;
                }
                else if(esc_parameters[0]==12){
                    //Text Cursor Disable Blinking
                    cursor_mode = CURSOR_TYPE_BLOCK_STEADY;
                }
                else if(esc_parameters[0]==1030){
                    // Set mouse cursor type
                    mouse_pointer = MOUSE_TYPE_OFF;
                }
            }
            break; // case 'l'

        case 'm':
            //Normal/Blinking/Reverse mode
            if(esc_parameters[0]==0){
                // Normal, all attributes off
                current_attr = TEXTATTR_OFF;
                bg_color = 0;
            }
            if(esc_parameters[0]==5){
                // Blinking on
                current_attr |= TEXTATTR_BLINK;
            }
            if(esc_parameters[0]==7){
                // Reverse on
                current_attr |= TEXTATTR_REVERSE;
            }

            // colors set directly with 30-37 (fg) or 40-47 (bg)
            if(esc_parameters[0]>=30 && esc_parameters[0]<=37){
                fg_color = esc_parameters[0]-30;
            }
            if(esc_parameters[0]>=40 && esc_parameters[0]<=47){
                bg_color = esc_parameters[0]-40;
            }

            //colors are selected via 38 (fg) or 48 (bg) and a parameter:
            //Next arguments are 5;n (Palette code 0-255)
            if(esc_parameters[0]==38 && esc_parameters[1]==5){
                if(esc_parameters[2]>=0 && esc_parameters[2]<=255){
                    fg_color = esc_parameters[2];
                }
            }
            if(esc_parameters[0]==48 && esc_parameters[1]==5){
                if(esc_parameters[2]>=0 && esc_parameters[2]<=255){
                    bg_color = esc_parameters[2];
                }
            }
            if(esc_parameters[0]==49 && esc_parameters[1]==5){
                if(esc_parameters[2]>=0 && esc_parameters[2]<=255){
                    screen_bg_color = esc_parameters[2];
                }
            }
            if(esc_parameters[0]==50){
                if(esc_parameters[1]>=0 && esc_parameters[1]<=255){
                  uint8_t c = esc_parameters[1];
                  if (esc_parameters[2]>=0 && esc_parameters[2]<=255 && esc_parameters[3]>=0 && esc_parameters[3]<=255 && esc_parameters[4]>=0 && esc_parameters[4]<=255) {
                    uint16_t rgb = ((esc_parameters[2] & 0b11111000) << 8) | ((esc_parameters[3] & 0b11111100) << 3) | (esc_parameters[4] >> 3);
                    display_pal_setcolor(c, rgb);
                  }
                }
            }
            // SPRITE Commands
            if(esc_parameters[0]==80){
                // ESC[80;n;em     | Set sprite state -e- enable and velocity -x- -y- 
                if(esc_parameters[1]>=0 && esc_parameters[1]<=MAX_SPRITE_NUM){
                    if (esc_parameters[2] == 1) { 
                      sprite_enable(esc_parameters[1],true);
                    } else { 
                      sprite_enable(esc_parameters[1],false);
                    }
                }
            }
            if(esc_parameters[0]==81){
                // ESC[81;n;x;ym       | Move sprite -n- to position -x- -y- 
                if(esc_parameters[1]>=0 && esc_parameters[1]<=MAX_SPRITE_NUM){
                    sprite_move(esc_parameters[1],esc_parameters[2],esc_parameters[3]);
                }
            }
            if(esc_parameters[0]==82){
                // ESC[82;n;x;ym     | Set sprite state -e- enable and velocity -x- -y- 
                if(esc_parameters[1]>=0 && esc_parameters[1]<=MAX_SPRITE_NUM){
                    sprite_velocity(esc_parameters[1],esc_parameters[2],esc_parameters[3]);
                }
            }
            break; // case 'm'

        case 'g':
            // GFX commands see gfx.ino
            if (esc_parameters[0]>0) {
              gfx_draw(esc_parameters[0],esc_parameters[1],esc_parameters[2],esc_parameters[3]);
            }
            break; // case 'g'

        case 's':
            // save cursor position
            saved_cursorx = cursorx;
            saved_cursory = cursory;
            break;

        case 'u':
            // move to saved cursor position
            cursorx = saved_cursorx;
            cursory = saved_cursory;
            break;

        case '7':
            // save cursor position
            saved_cursorx = cursorx;
            saved_cursory = cursory;
            reset_escape_sequence();
            break;
            
        case '8':
            // move to saved cursor position
            cursorx = saved_cursorx;
            cursory = saved_cursory;
            reset_escape_sequence();
            break;

        case 'J':
            // Clears part of the screen. If n is 0 (or missing), clear from cursor to end of screen.
            // If n is 1, clear from cursor to beginning of the screen. If n is 2, clear entire screen
            // (and moves cursor to upper left on DOS ANSI.SYS).
            // If n is 3, clear entire screen and delete all lines saved in the scrollback buffer
            // (this feature was added for xterm and is supported by other terminal applications).
            switch(esc_parameters[0]){
                case 0:
                  // clear from cursor to end of screen
                  display_tb_clear_from(cursorx,cursory);
                  break;

                case 1:
                  // clear from cursor to beginning of the screen
                  display_tb_clear_to(cursorx,cursory);
                  break;

                case 2:
                  display_tb_clear();
                  cursorx=0;
                  cursory=0;
                  break;

                case 3:
                  display_tb_clear();
                  cursorx=0;
                  cursory=0;
                  break;
            }
            break; // case 'J'


        case 'K':
            // Erases part of the line. If n is 0 (or missing), clear from cursor to the end of the line.
            // If n is 1, clear from cursor to beginning of the line. If n is 2, clear entire line.
            // Cursor position does not change.
            switch(esc_parameters[0]){
                case 0:
                  // clear from cursor to the end of the line
                  display_tb_clear_line_from(cursorx,cursory);
                  break;

                case 1:
                  // clear from cursor to beginning of the line
                  display_tb_clear_line_to(cursorx,cursory);
                  break;

                case 2:
                  // clear entire line
                  display_tb_clear_line(cursorx,cursory);
                  break;
            }
            break; // case 'K'


        case 'A': // Cursor Up - Moves the cursor n (default 1) cells
            n = esc_parameters[0];
            if (n==0) n=1;
            cursory -= n;
            if (cursory <0) { 
              cursory = 0;
            }
            break;

        case 'B': // Cursor Down - Moves the cursor n (default 1) cells
            n = esc_parameters[0];
            if (n==0) n=1;
            cursory += n;
            if (cursory > TEXTSY-1) { 
              cursory = TEXTSY-1;
            }
            break;

        case 'C': // Cursor Forward - Moves the cursor n (default 1) cells
            n = esc_parameters[0];
            if (n==0) n=1;
            cursorx += n;
            if (cursorx > TEXTSX-1) { 
              cursorx = TEXTSX-1;
            }
            break;

        case 'D': // Cursor Backward - Moves the cursor n (default 1) cells
            n = esc_parameters[0];
            if (n==0) n=1;
            cursorx -= n;
            if (cursorx <0) { 
              cursorx = 0;
            }
            break;

        case 'S': // Scroll whole page up by n (default 1) lines. New lines are added at the bottom. (not ANSI.SYS)
            n = esc_parameters[0];
            if (n==0) n=1;
            for(int i=0;i<n;i++)
              display_tb_scroll();
            break;

        case 'q': // Set cursor type
            if(parameter_sp){
                parameter_sp = false;
                // Parameter0 corresponds to CURSOR_TYPE_xxx in ansi_esc.ino
                cursor_mode = esc_parameters[0];
            }
            break;

        case 'n': // Query state of terminal
            if (esc_parameters[0]==5){         // Return Terminal state
                Serial1.printf("\e[0n");       // always OK 
            }
            if (esc_parameters[0]==6){         // Return Cursor position
                DEBUG_PRINT("Cursor Position x:%i  y:%i",cursorx,cursory);
                Serial1.printf("\e[%03d;%03dR",cursory,cursorx);
            }
            if (esc_parameters[0]==9){         // Return Mouse position and buttons
                DEBUG_PRINT("Mouse Position x:%i  y:%i",mousex,mousey);
                Serial1.printf("\e[%03d;%03d;%03dR",mousey,mousex,mousebuttons);
            }

            break;

         case 'c': // Reset Terminal
            display_reset();
            reset_escape_sequence();
            break;

/*
        // Does U is part of VT100 ?
        case 'U':
            if(parameter_q){
                // user-defined character data follows
                // first parameter is the character number, which must be 128-255 inclusive
                // follow with 8 bytes which will be inserted into the UD character space

                data_purpose=UDCHAR;
                current_udchar = esc_parameters[0];
                data_bytes_expected = 8;
            }
            break; // case 'U'
*/
    } // eof switch(esc_final_byte)
  } // eof if(esc_c1=='[')
  else{
    // ignore everything else
  }
  // our work here is done
  reset_escape_sequence();
}

void handle_udchar_data(uint8_t d){
/*    data_bytes_expected--;
    // doing this first makes it 7 - 0
    static int bytenum;
    bytenum = 7-data_bytes_expected;
    //to make 0-7
    bytenum+=((current_udchar-128)*8);

    custom_bitmap[bytenum] = d;
*/
}

void handle_new_character(char chr){
  // Ask Terminal core to handle this new character
  
  // === data? ============================================================
/*  if(data_bytes_expected>0){
        if(data_purpose==ARBITRARY){

        }
        if(data_purpose==BITMAPDATA){

        }
        if(data_purpose==UDCHAR){
            handle_udchar_data((uint8_t)asc);
        }
  }
  else */

  // === ESC Sequences ? ================================================
  if  ( esc_state != ESC_READY ) {
    // Handle ESC sequences
    switch(esc_state){
        case ESC_ESC_RECEIVED:
            // DEBUG_PRINT("ESC Received:  %c  %i  0x%02x\n\r",chr,chr,chr);
            // --- waiting on c1 character ---
            // c1 is the first parameter after the ESC
            if( (chr=='(') || (chr=='[') || (chr==']') ){
                // 0x9B = CSI, that's the only one we're interested in atm
                // the others are 'Fe Escape sequences'
                // usually two bytes, ie we have them already.
                if(chr=='['){    // ESC+[ =  0x9B){
                    esc_c1 = chr;
                    esc_state=ESC_PARAMETER_READY; // Lets wait for parameter
                    clear_escape_parameters();
                    // number of expected parameter depends on next caracter.
                }
                else if(chr=='('){    // ESC+(
                    esc_c1 = chr;
                    esc_state=ESC_PARAMETER_READY; // Lets wait for parameter
                    clear_escape_parameters();
                    parameter_p=true; // we expect a single parameter.
                }
                else if(chr==']'){    // ESC+]
                    esc_c1 = chr;
                    esc_state=ESC_PARAMETER_READY; // Lets wait for parameter
                    clear_escape_parameters();
                    parameter_r=true; // we expect a string parameter 
                }
                // other type Fe sequences go here
                else
                    // for now, do nothing
                    reset_escape_sequence();
            }
            else
                // unrecognised character after escape.
                reset_escape_sequence();
            break;

        case ESC_PARAMETER_READY:
            // waiting on parameter character, semicolon or final byte
            // DEBUG_PRINT("ESC Parameter: %c  %i  0x%02x\n\r",chr,chr,chr);
            if(!parameter_r) {
                // Handle normal ESC [ sequences
                if(chr>='0' && chr<='9'){
                    if(parameter_p){
                      // final byte. Log and handle
                      esc_final_byte = chr;
                      esc_sequence_received(); // execute esc sequence
                    }
                    else {
                        // get parameter value
                        if(esc_parameter_count<MAX_ESC_PARAMS){
                            unsigned char digit_value = chr - 0x30; // '0'
                            esc_parameters[esc_parameter_count] *= 10;
                            esc_parameters[esc_parameter_count] += digit_value;
                        }
                    } 
                }
    
                else if(chr==';'){
                      // move to next param
                      if(esc_parameter_count<MAX_ESC_PARAMS) esc_parameter_count++;
                }
                else if(chr=='?'){
                    parameter_q=true;
                }
                else if(chr==' '){
                    parameter_sp=true;
                }
                else if(chr>=0x40 && chr<0x7E){
                    // final byte. Log and handle
                    esc_final_byte = chr;
                    esc_sequence_received(); // execute esc sequence
                }
                else{
                    // unexpected value, undefined
                }
            } else {
                // Handle OS ESC ] sequences
                if(chr==';' || chr<0x20 ) {
                  //DEBUG_PRINT("ESC OS Command String finished: [%s]\n\r",esc_os_str);
                  filesystem_cmd_sequence_received(esc_os_str);
                  reset_escape_sequence();
                } else {
                    if(chr>=0x20 && chr<0x7E){
                       char temp[2];
                       temp[0]=chr;
                       temp[1]=0x00;
                       strncat(esc_os_str, temp, sizeof(esc_os_str)-1 );
                       //DEBUG_PRINT("char: %i %x\n\r", chr, chr);
                    }
                }
            }
            break;
    }
  } else {

    // === Control Characters ? ==============================================
    switch (chr) {
       case ASCII_FF:   // Clear Screen
            display_tb_clear();
            cursorx = 0;
            cursory = 0;
            break;
       case ASCII_CR:   // Carriage Return 
            cursorx = 0;
            break;
       case ASCII_LF:   // Line Feed
            cursory += 1;
            if (cursory > TEXTSY-1) { 
              display_tb_scroll();
              cursory = TEXTSY-1;
            }
            break;
       case ASCII_BS:   // Backspace
            if (cursorx>0) {
              cursorx -= 1;
              int i=cursorx+cursory*TEXTSX;
              text_buf[i]=32;
            }
            break;
       case ASCII_BELL: // Bell Signal
            beeper_on = true;
            break;    
       case ASCII_ESC:  // ESC Sequence start
            esc_state = ESC_ESC_RECEIVED;
            break;
            
    // === Or normal charachters ? ===========================================
    // we store normal characters into the textbuffer with the currently 
    // set attributes, colors, and scroll if necessary
       default:   
            text_buf[cursorx + cursory*TEXTSX]  = chr;
            text_fg[cursorx + cursory*TEXTSX]   = fg_color;
            text_bg[cursorx + cursory*TEXTSX]   = bg_color;
            text_attr[cursorx + cursory*TEXTSX] = current_attr;
            cursorx += 1;
            if (cursorx >TEXTSX-1) { 
              cursorx=0; 
              cursory+=1;
              if (cursory >TEXTSY-1) { 
                display_tb_scroll();
                cursory = TEXTSY-1;
              }
            }
    }
  }
}
