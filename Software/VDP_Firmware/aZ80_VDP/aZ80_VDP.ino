//
// aZ80 Pi Pico 2 VDP Terminal Emulator
//
// by Frank Hellmann, 2023, 2024, 2025
//
// for use with the attraktor.org aZ80 system 
// compatible with RC2014 systems
//
// includes: 
// - basic Terminal Emulation VT100
// - USB Host support for keyboard and mouse
// - TTL Serial comunication with 57600 baud
// - TTL Serial debug port with 115200 baud
// - Double-buffered 8-bit Adafruit_GFX-compatible framebuffer for PicoDVI
//   Allows animation without redraw flicker. Requires Adafruit_GFX >= 1.11.4
// - Simple Sprite Engine via serial ANSI ESC commands (see ansi_esc tab)
// - Simple Graphics Commands via serial ANSI ESC commands (see ansi_esc tab)
//

// Theory of operation:
// We have two modes to operate in: 
// mode 0: 320x240 Pixels with Terminal & Sprites enabled (Default)
// mode 1: 320x240 Pixels with GFX buffer & Sprites enabled
// mode 2: 320x240 Pixels with GFX buffer & Terminal & Sprites enabled
//
// In display.ino the layering of the buffers on top of each other is set to:
// - GFX buffer is background/last layer if enabled
// - Text buffer if enabled
// - Sprites
// - Status Display on top if enabled
// - Mouse Cursor


// Support for RP2040 has been DROPPED due to lack of memory for GFX buffer!
// !!Needed Arduino Options RP2040:
// !!- Install Raspberry Pi Pico Boards, Adafruit_PicoDVI and Pi Pico USB with dependencies. Then:
// !!- Tools -> Board -> Raspberry Pi Pico     (Raspberry Pi Pico W currently does not work)
// !!- Tools -> Flash size -> 2MB (no FS)
// !!- Tools -> Optimize -> Optimize even more (-O3)  to fix red lines apearing
// !!- Tools -> USB Stack -> Adafruit TinyUSB  (We need a fix here -> see usbhost.ino)
//

// Needed Arduino Options RP2350:
// - Install Raspberry Pi Pico 2 Boards, Adafruit_PicoDVI and Pi Pico USB with dependencies. Then:
// - Tools -> Board -> Raspberry Pi Pico 2   (Raspberry Pi Pico 2W currently does not work)
// - Tools -> CPU Speed -> 250MHz (To solve serial baudrate miscalculation)
// - Tools -> Flash size -> 1MB (3MB FS)
// - Tools -> Optimize -> Optimize even more (-O3)  to fix red lines apearing
// - Tools -> USB Stack -> Adafruit TinyUSB Host (native)
//

/* Pin Assignments
 * 
 *                                      _____|----|_____
 *                     aZ80 RX   GP0 - |      USB       | - VBUS
 *                     aZ80 TX   GP1 - |                | - VSYS     5V
 *                               GND - | * LED/GP25     | - GND
 *                   --    DB0   GP2 - |                | - 3V3_EN
 *                   |     DB1   GP3 - |                | - 3V3 OUT
 *                   |     DB2   GP4 - |                | - ADC_VREF
 *                   |     DB3   GP5 - |                | - GP28     AD2
 *                   |           GND - |                | - GND
 *          aZ80 Bus |     DB4   GP6 - |   Raspberry    | - GP27     AD1
 *                   |     DB5   GP7 - |       Pi       | - GP26     AD0   
 *                   |     DB6   GP8 - |     Pico 2     | - RUN      Reset Button
 *                   |     DB7   GP9 - |                | - GP22     Beeper O/P
 *                   |           GND - |                | - GND
 *                   |     RW   GP10 - |                | - GP21     RX2 Debug
 *                   --    CS   GP11 - |                | - GP20     TX2 Debug
 *                     DVI D0+  GP12 - |                | - GP19     DVI D1-
 *                     DVI D0-  GP13 - |                | - GP18     DVI D1+
 *                               GND - |                | - GND
 *                    DVI CLK+  GP14 - |                | - GP17     DVI D2-
 *                    DVI CLK-  GP15 - |___--__--__--___| - GP16     DVI D2+
 *                             SWCLK ______/  GND   \______ SWDIO
 */

// Todo ( - planned, + done ):
// + Serial Terminal Emu     aZ80 -> VDP darstellen
// + Keyboard Emu            VPD  -> aZ80 Tasten senden
// + ANSI Emu
//   + Mouse Cursor on/off, Position R/W
//   + Text Cursor on/off, Position R/W
//   + Palette
//   + Set palette entry with RGB values
//   - Smooth scrolling, extended Fontbuffer
//   - Upload Font Bitmaps
//   - Upload Mouse Pointer Bitmaps
// + Sprites
//   + draw sprites layer
//   + Set position, velocity
//   - Upload Sprite Bitmaps
//   - Report Collisions
// + GFX Layer Commands
//   + implement basic GFX commands
//   + draw GFX layer
//   - Smooth scrolling
//   - Blitter operations
//

// Do we want debugging over Serial Port 2 ?
// uncomment #define if we do
// (be aware that this can impact performace and framerate visibly)
//#define DEBUG

#ifdef DEBUG
 #define DEBUG_PRINT(...) Serial2.printf(__VA_ARGS__)
#else
 #define DEBUG_PRINT(...)
#endif

// Default Serial Speeds
#define SER1_SPEED  57600
#define SER2_SPEED 115200

// SdFat 2.2.1 or later Adafruit Fork is needed
// SdFat is required for using Adafruit_USBH_MSC_SdFatDevice
// (might clash with ESP8266_SdFat Library from Pi Pico Board due to same naming)
#include "SdFatConfig.h"
#include "SdFat.h"

// TinyUSB lib
#include "Adafruit_TinyUSB.h"

// USB Host object
Adafruit_USBH_Host USBHost;
// USB Host MSC Block Device object which implemented API for use with SdFat
Adafruit_USBH_MSC_BlockDevice msc_block_dev;

// DVI object
#include <PicoDVI.h>
DVIGFX8 display(DVI_RES_320x240p60, true, pico_sock_cfg);
#define DISPLAYSX 320
#define DISPLAYSY 240

// Default Runtime Configuration
static bool is_keyboard_mounted = false;
static bool is_mouse_mounted = false;
static bool is_joystick_mounted = false;
static bool is_filesystem_mounted = false;

// Check for WiFi capabilities ( not used so far )
static bool is_wifi_capable = rp2040.isPicoW();

// Textbuffer ( 8x8 font -> 40x30 characters = 4,8kB)
#define TEXTSX 40
#define TEXTSY 30 
#define TEXTBUFSZ 40*30
static uint8_t text_buf[TEXTBUFSZ];
static uint8_t text_fg[TEXTBUFSZ];
static uint8_t text_bg[TEXTBUFSZ];
static uint8_t text_attr[TEXTBUFSZ];
#define TEXTATTR_OFF      0
#define TEXTATTR_BLINK    1
#define TEXTATTR_REVERSE  2

// Display
static uint8_t  screen_mode     = 2;
static uint8_t  screen_bg_color = 0;
static uint8_t  fg_color        = 7;
static uint8_t  bg_color        = 0;
static uint8_t  current_attr    = 0;
static uint8_t  cycle_speed     = 3;
static uint8_t  cycle_count     = 0;
static uint16_t sprite_gui      = 0;
static bool     status_gui_on   = false;
static uint64_t cpu_ticks       = 0;

// Cursor
static unsigned int  cursor_mode = 0;
static unsigned int  cursorx = 0;
static unsigned int  cursory = 0;
static unsigned int  saved_cursorx = 0;
static unsigned int  saved_cursory = 0;

// Mouse
static unsigned int  mouse_pointer = 0;
static unsigned int  mousebuttons  = 0;
static unsigned int  mousex = 0;
static unsigned int  mousey = 0;

// Beeper
#define BEEPER_PIN 22
#define BEEPER_LOOPS 3
bool beeper_on = false;
int  beeper_timer = 0;

// GFX (64kB)
// allocate one buffer for graphics 320x240
GFXcanvas8       gfx_buffer(DISPLAYSX, DISPLAYSY);
static uint16_t  gfx_curx = 0;
static uint16_t  gfx_cury = 0;
static uint8_t   gfx_fgcol = fg_color;
static uint8_t   gfx_bgcol = bg_color;

// Sprites  ( 32kB )
#define MAX_SPRITE_NUM 63
struct SpriteData {
  int16_t   xpos;
  int16_t   ypos;
  int8_t    xvel;
  int8_t    yvel;
  uint8_t   xsize;
  uint8_t   ysize;
  bool      enabled;
  uint8_t   data[576];
  uint8_t   mask[72];
};
static SpriteData Sprite[MAX_SPRITE_NUM];

/* 
 *  Setup & Loop
*/
void setup() {
  // Setup Status LED on Pi Pico
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,1);   

  // setup UART
  serial_setup();
  
  // start USB Host
  usbhost_setup();

  // setup Beeper Signal
  beeper_setup();

  // setup GFX
  gfx_setup();

  // setup Sprites
  sprite_setup();

  // setup Ansi Decoder
  ansi_setup();

  // setup DVI Display
  display_setup();
}

// Core0: loops our stuff
// Core1: is used for generating the DVI Signal
void loop() {
  // update stuff
  serial_update();
  usbhost_update();
  beeper_update();
  sprite_update();
  display_update();

  // blink LED
  digitalWrite(LED_BUILTIN, (millis() / 500) & 1);    
}

//
// TinyUSB callbacks need to be in main .ino and get routed to usbhost.ino
//
// Invoked when device with hid interface is mounted
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len) {
  usbhost_hid_mount(dev_addr, instance, desc_report, desc_len);
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
  usbhost_hid_umount(dev_addr, instance);
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
  usbhost_hid_report_received(dev_addr, instance, report, len);
}

// Invoked when a device with MassStorage interface is mounted
void tuh_msc_mount_cb(uint8_t dev_addr)
{
  usbhost_msc_mount_cb(dev_addr);
}

// Invoked when a device with MassStorage interface is unmounted
void tuh_msc_umount_cb(uint8_t dev_addr)
{
  usbhost_msc_umount_cb(dev_addr);
}
