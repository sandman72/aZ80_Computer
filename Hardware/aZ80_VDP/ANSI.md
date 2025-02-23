### We support the following ESC Sequences

To use them the Escape Sequence (ESC = 27) needs to be send. For example:

    PRINT CHR$(27);"[7m REVERSE"
    PRINT CHR$(27);"[31m RED"
  

#### Single Chars:
|:-----|:-----|
| ESC[7               | Save the cursor position |
| ESC[8               | Move cursor to previously saved position |
| ESC[c               | Reset Terminal |
| ESC[s               | Save the cursor position |
| ESC[u               | Move cursor to previously saved position |
| ESC[A               | Move the cursor up |
| ESC[B               | Move the cursor down |
| ESC[C               | Move the cursor forward |
| ESC[D               | Move the cursor backward |
| ESC[H               | Move to 0-0 |
| ESC[J               | Clear screen from cursor |
| ESC[K               | Clear line from cursor |

#### Multiple chars:

|:-----|:-----|
| ESC[?12l            | Cursor not blinking |
| ESC[?12h            | Cursor blinking |
| ESC[?25l            | Cursor invisible |
| ESC[?25h            | Cursor visible |
| ESC[0m              | Text Normal |
| ESC[5m              | Text Blink on |
| ESC[7m              | Text Reverse on |
| ESC[30m             | Set foreground color to palette entry 0 (usually black) |
|  to                 | |
| ESC[37m             | Set foreground color to palette entry 7 (usually white) |
| ESC[38;5;-n-m       | Set foreground color to palette entry -n- (0-255) |
| ESC[40m             | Set background color to palette entry 0 (usually black) |
|  to
| ESC[47m             | Set background color to palette entry 7 (usually white) |
| ESC[48;5;-n-m       | Set background color to palette entry -n- (0-255) |
| ESC[49;5;-n-m       | Set screen background color to palette entry -n- (usually black) |
| ESC[50;c;r;g;bm     | Set palette entry -c- to RGB color -r-,-g-,-b- (0-255) |
| ESC[5n              | Return terminal status "OK" |
| ESC[6n              | Return cursor position    -> returns ESC[-n-;-m-R   -n- = y   -m- = x     (reversed) |
| ESC[9n              | Return mouse pos/buttons  -> returns ESC[-n-;-m-;-b-R   -n- = x   -m- = y  -b- = buttons |
| ESC[-n-A            | Move the cursor up -n- lines |
| ESC[-n-B            | Move the cursor down -n- lines |
| ESC[-n-C            | Move the cursor forward -n- characters |
| ESC[-n-D            | Move the cursor backward -n- characters |
| ESC[-Row-;-Col-H    | Move to -Row-,-Col- |
| ESC[0J              | clear screen from cursor |
| ESC[1J              | clear screen to cursor |
| ESC[2J              | Clear the screen and move the cursor to 0-0 |
| ESC[3J              | same as ESC[2J |
| ESC[0K              | Clear from cursor to the end of the line |
| ESC[1K              | Clear from the beginning of the current line to the cursor |
| ESC[2K              | Clear the whole line |
| ESC[nS              | scroll whole page up by n rows (default 1 if n missing) |

#### Sprites

|:-----|:-----|
| ESC[80;n;em         | Set sprite state -e- enable=1 disable=0 |
| ESC[81;n;x;ym       | Move sprite -n- to position -x- -y- |
| ESC[82;n;x;ym       | Set sprite velocity to -x- -y- |

#### Graphics

|:-----|:-----|
| ESC[1;0;0;g         | Move GFX cursor to position x=0 y=0 |
| ESC[5;10;20;42;g    | Draw Pixel at position x=10 y=20 with color 42 |
| ESC[10;80;90;17;g   | Draw Rectangle at cursor with width 80, height 90 with color 17 |
| ESC[11;80;90;17;g   | Draw filled Rectangle at cursor with width 80, height 90 with color 17 |
| ESC[20;40;64;g      | Draw Circle at cursor with radius 40 with color 64 |
| ESC[21;40;64;g      | Draw filled Circle at cursor with radius 40 with color 64 |
| ESC[99g             | Clear command list and set cursor to x=0 y=0 |

#### Cursor Modes

|:-----|:-----|
| ESC[0 q             | Cursor Off |
| ESC[1 q             | Blinking Block |
| ESC[2 q             | Steady Block |
| ESC[3 q             | Blinking Underline |
| ESC[4 q             | Steady Underline |
| ESC[5 q             | Blinking Bar |
| ESC[6 q             | Steady Bar |

#### Mouse Modes

|:-----|:-----|
| ESC[?1020h           | Return Mouse Position -> returns ESC[-n-;-m-;-b-R   -n- = x  -m- = y  -b- = buttons |
| ESC[?1030h           | Mouse Off |
| ESC[?1031h           | Mouse moves text cursor (TODO) |
| ESC[?1032h           | Mouse shows as single point |
| ESC[?1033h           | Mouse shows as cross hair |
| ESC[?1034h           | Mouse shows as user defined sprite (TODO) |
| ESC[?1030l           | Mouse Off |

#### OS Commands (TODO)
commands are send in form of ESC]command; with a closing ;

|:-----|:-----|
| ESC]dir;             | Return directory listing of USB Stick |
| ESC]load -n-;        | Loads/Prints out file -n- to terminal |
| ESC]save -n-;        | Saves terminal output to file -n- |
| ESC]del -n-;         | Deletes file -n- |
