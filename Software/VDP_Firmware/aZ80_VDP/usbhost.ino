//
// !!! TinyUSB Lib before 3.3.x needs modification:  !!!
//
// C:\HomeDir\AppData\Local\Arduino15\packages\rp2040\hardware\...
//      rp2040\3.x.x\libraries\Adafruit_TinyUSB_Arduino\...
//      src\arduino\ports\rp2040\tusb_config_rp2040.h
//
// // Enable device stack
// #define CFG_TUD_ENABLED 1
//
// // Enable host stack with pio-usb if Pico-PIO-USB library is available
// //#if __has_include("pio_usb.h")
// //#define CFG_TUH_ENABLED 1
// //#define CFG_TUH_RPI_PIO_USB 1
// //#endif
// #define CFG_TUH_ENABLED 1
// #define CFG_TUH_RPI_PIO_USB 0
// #define CFG_TUH_MSC 1
//

//--------------------------------------------------------------------+
// Setup and Loop on Core 0
//--------------------------------------------------------------------+

void usbhost_setup() {
  DEBUG_PRINT("\r\n");
  DEBUG_PRINT("Setup to TinyUSB Host\r\n");
  // run host stack
  USBHost.begin(0);
}

void usbhost_update()
{
  USBHost.task();
}

//--------------------------------------------------------------------+
// Keyboard uses keycode tables from keyboard.ino
//--------------------------------------------------------------------+
// US:  static uint8_t const keycode2ascii[128][3] =  { HID_KEYCODE_TO_ASCII_US };
// UK:  static uint8_t const keycode2ascii[128][3] =  { HID_KEYCODE_TO_ASCII_UK };
// DE:  static uint8_t const keycode2ascii[128][3] =  { HID_KEYCODE_TO_ASCII_DE };
// FR:  static uint8_t const keycode2ascii[128][3] =  { HID_KEYCODE_TO_ASCII_FR };

// select keyboard layout here:
static uint8_t const keycode2ascii[128][3] =  { HID_KEYCODE_TO_ASCII_US };

static hid_keyboard_report_t prev_kbd_report = { 0, 0, {0} }; // previous report to check key released

// Keyboard Capslock
static bool capslock_key_down_in_this_report = false;
static bool capslock_on = true;

// Keyboard LED control
static uint8_t keyb_leds = KEYBOARD_LED_CAPSLOCK;
static uint8_t keyb_prev_leds = 0xFF;

// look up new key in previous keys
static inline bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode)
{
  for(uint8_t i=0; i<6; i++)
  {
    if (report->keycode[i] == keycode)  return true;
  }

  return false;
}

static void process_kbd_report(hid_keyboard_report_t const *report)
{
  //------------- example code ignore control (non-printable) key affects -------------//
  for(uint8_t i=0; i<6; i++)
  {
    if ( report->keycode[i] )
    {
      // check for Capslock key
      capslock_key_down_in_this_report = false;
      for(uint8_t i=0; i<6; i++){
          if ( find_key_in_report(report, HID_KEY_CAPS_LOCK)){
            capslock_key_down_in_this_report = true;
          }
      }
      if ( capslock_key_down_in_this_report ){
          // toggle the value
          capslock_on = !capslock_on;
          if ( capslock_on ) {
            keyb_leds |= KEYBOARD_LED_CAPSLOCK;
          } else {
            keyb_leds &= ~KEYBOARD_LED_CAPSLOCK;
          }
      }
            
      if ( find_key_in_report(&prev_kbd_report, report->keycode[i]) )
      {
        // exist in previous report means the current key is holding
        // TODO: repeat keys
      } else
      {
        // not existed in previous report means the current key is pressed
        bool const is_shift = report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT);
        bool const is_ctrl  = report->modifier & (KEYBOARD_MODIFIER_LEFTCTRL  | KEYBOARD_MODIFIER_RIGHTCTRL);
        bool const is_altgr = report->modifier & (KEYBOARD_MODIFIER_RIGHTALT);
        bool const is_gui   = report->modifier & (KEYBOARD_MODIFIER_LEFTGUI | KEYBOARD_MODIFIER_RIGHTGUI );
        uint8_t ch=0;

        // check for left GUI key (usually Windows Key)
        uint8_t keyc = report->keycode[i];
        if ( is_gui )
        {
          ch = keycode2ascii[keyc][0];
          if ( ch == ASCII_ESC ) {          
            status_gui_on = !status_gui_on;
          }
        } else {
          if (!is_ctrl && !is_altgr) {
            // normal keycodes are send to the terminal
            if (capslock_on && keyc < 0x1e) {
              ch = keycode2ascii[keyc][is_shift ? 0 : 1];          
            } else {
              ch = keycode2ascii[keyc][is_shift ? 1 : 0];          
            }
          } else {
            if (is_ctrl && !is_altgr) {
              // if control key is pressed send CTRL-*
              ch = keycode2ascii[report->keycode[i]][1] - 64;
              // make sure only valid CTRL chars are send
              if ( ch > 31 ) ch = 0;
            }
            if (!is_ctrl && is_altgr) {
              // if right altgr key is pressed send to the terminal
              ch = keycode2ascii[report->keycode[i]][2];
            }
          }
          
          DEBUG_PRINT("[ %c | %i | 0x%02x  kc: %i | 0x%02x]",ch,ch,ch,keyc,keyc);
          // send char to serial port 1
          if (ch > 0 ) {
            // normal stuff goes to serial
            Serial1.printf("%c",ch);
            if (ch == '\r') Serial1.print("\n");
          } else {
            // special keycodes go here and then to serial
            if (keyc == SCANCODE_UP )    Serial1.print("\e[A");
            if (keyc == SCANCODE_DOWN )  Serial1.print("\e[B");
            if (keyc == SCANCODE_RIGHT ) Serial1.print("\e[C");
            if (keyc == SCANCODE_LEFT )  Serial1.print("\e[D");
            if (keyc == SCANCODE_HOME )  Serial1.print("\e[H");
          }
          Serial1.flush();
        }
      }
    }
    // TODO example skips key released
  }
  prev_kbd_report = *report;
}

//--------------------------------------------------------------------+
// Mouse
//--------------------------------------------------------------------+
#define MOUSE_SCALING 5
static hid_mouse_report_t prev_mouse_report = { 0, 0, 0, 0, 0 };

void cursor_movement(int8_t x, int8_t y, int8_t wheel)
{
  (void)wheel;
  DEBUG_PRINT("Mouse: %d %d %d  ", x, y, wheel);
  mousex += int( x / MOUSE_SCALING);
  mousey += int( y / MOUSE_SCALING);
  if (mousex < 0) mousex = 0;
  if (mousex > DISPLAYSX-1) mousex = DISPLAYSX-1;
  if (mousey < 0) mousey = 0;
  if (mousey > DISPLAYSY-1) mousey = DISPLAYSY-1;
}

static void process_mouse_report(hid_mouse_report_t const * report)
{
  //------------- cursor movement -------------//
  cursor_movement(report->x, report->y, report->wheel);
  // BUG Sometimes TinyUSB report->wheel holds Y movement
  //cursor_movement(report->x, report->wheel, report->wheel);

  //------------- button state  -------------//
  uint8_t button_changed_mask = report->buttons ^ prev_mouse_report.buttons;
  if ( button_changed_mask & report->buttons)
  {
    DEBUG_PRINT(" %c%c%c ", 
        report->buttons & MOUSE_BUTTON_LEFT   ? 'L' : '-',
        report->buttons & MOUSE_BUTTON_MIDDLE ? 'M' : '-',
        report->buttons & MOUSE_BUTTON_RIGHT  ? 'R' : '-');
    mousebuttons = report->buttons;
  }
}

//--------------------------------------------------------------------+
// Gamepad
//--------------------------------------------------------------------+
static uint8_t joystick_state = 0;
static uint8_t prev_joystick_state = 0xff;

static void process_joystick_report( uint8_t const * report ) {
  // TODO: 
  // This is just a hack to get the joystick working, not sure if this
  // will work with other gamepads and josyticks.
  // tested with USB "THE C64" mini joystick
  
  joystick_state = 0;    // Bits set in this order: 
  if (report[0] == 0x00) { joystick_state |= 1;  } // "LEFT " 
  if (report[0] == 0xff) { joystick_state |= 2;  } // "RIGHT " 
  if (report[1] == 0x00) { joystick_state |= 4;  } // "UP "   
  if (report[1] == 0xff) { joystick_state |= 8;  } // "DOWN " 
  if (report[5] == 0x1F) { joystick_state |= 16; } // "Fire1 "   
  if (report[5] == 0x2F) { joystick_state |= 32; } // "Fire2 " 
  if (report[5] == 0x4F) { joystick_state |= 64; } // "Fire3 "   
  if (report[5] == 0x8F) { joystick_state |= 128;} // "Fire4 " 
}

//--------------------------------------------------------------------+
// HID related functions
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use.
// tuh_hid_parse_report_descriptor() can be used to parse common/simple enough
// descriptor. Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE,
// it will be skipped therefore report_desc = NULL, desc_len = 0
void usbhost_hid_mount(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len) {
  (void)desc_report;
  (void)desc_len;
  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  DEBUG_PRINT("HID device address = %d, instance = %d is mounted\r\n", dev_addr, instance);
  DEBUG_PRINT("VID = %04x, PID = %04x\r\n", vid, pid);
  if (!tuh_hid_receive_report(dev_addr, instance)) {
    DEBUG_PRINT("Error: cannot request to receive report\r\n");
  } else { 
    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
    switch (itf_protocol)
    {
      case HID_ITF_PROTOCOL_KEYBOARD:
        DEBUG_PRINT("USB Keyboard mounted.\r\n");
        is_keyboard_mounted = true;
        tuh_hid_set_report(dev_addr, instance, 0, HID_REPORT_TYPE_OUTPUT, &keyb_leds, sizeof(keyb_leds));
      break;
  
      case HID_ITF_PROTOCOL_MOUSE:
        DEBUG_PRINT("USB Mouse mounted.\r\n");
        is_mouse_mounted = true;
      break;
  
      default:
        DEBUG_PRINT("USB Generic mounted.\r\n");
        //TODO: find out if its a real joystick/gamepad
        is_joystick_mounted = true;
      break;
    }
  }
}

// Invoked when device with hid interface is un-mounted
void usbhost_hid_umount(uint8_t dev_addr, uint8_t instance) {
  DEBUG_PRINT("HID device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
  switch (itf_protocol)
  {
    case HID_ITF_PROTOCOL_KEYBOARD:
      DEBUG_PRINT("USB Keyboard unmounted.\r\n");
      is_keyboard_mounted = false;
    break;

    case HID_ITF_PROTOCOL_MOUSE:
      DEBUG_PRINT("USB Mouse unmounted.\r\n");
      is_mouse_mounted = false;
    break;

    default:
      DEBUG_PRINT("USB Generic unmounted.\r\n");
      //TODO: find out if its a real joystick/gamepad
      is_joystick_mounted = false;
    break;
  }
}

// Invoked when received report from device via interrupt endpoint
void usbhost_hid_report_received(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
  /*DEBUG_PRINT("HIDreport : ");
  for (uint16_t i = 0; i < len; i++) {
    DEBUG_PRINT("0x%02X ", report[i]);
  }
  DEBUG_PRINT("\r\n");*/ 
   
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
  switch (itf_protocol)
  {
    case HID_ITF_PROTOCOL_KEYBOARD:
      // Keyboard
      DEBUG_PRINT("(Keyboard) ");
      process_kbd_report( (hid_keyboard_report_t const*) report );
      if (keyb_leds != keyb_prev_leds) {
        tuh_hid_set_report(dev_addr, instance, 0, HID_REPORT_TYPE_OUTPUT, &keyb_leds, sizeof(keyb_leds));
        keyb_prev_leds = keyb_leds;
      }
      DEBUG_PRINT("\r\n");
    break;

    case HID_ITF_PROTOCOL_MOUSE:
      // Mouse
      DEBUG_PRINT("(Mouse) ");
      process_mouse_report( (hid_mouse_report_t const*) report );
      DEBUG_PRINT("\r\n");
    break;

    default:
      // Generic - we use this for joysticks and gamepads
      if ( len == 8 ) {
        // at least size matches, as we do not support other stuff we'll use this for gamepad/joystick
        process_joystick_report( report );
        if (prev_joystick_state != joystick_state) {
          DEBUG_PRINT("(Joystick) %i", joystick_state);
          prev_joystick_state = joystick_state;
          DEBUG_PRINT("\r\n");
        }
      } else {
        DEBUG_PRINT("(Generic %i) ",len);
        // TODO: process_generic_report(dev_addr, instance, report, len);
        DEBUG_PRINT("\r\n");
      }
    break;
  }
  
  // continue to request to receive report
  if (!tuh_hid_receive_report(dev_addr, instance)) {
    DEBUG_PRINT("Error: cannot request to receive report\r\n");
  }
}

// Invoked when a device with MassStorage interface is mounted
void usbhost_msc_mount_cb(uint8_t dev_addr)
{
  // Initialize block device with MSC device address
  msc_block_dev.begin(dev_addr);

  // For simplicity this example only support LUN 0
  msc_block_dev.setActiveLUN(0);

  filesystem_mount();
}

// Invoked when a device with MassStorage interface is unmounted
void usbhost_msc_umount_cb(uint8_t dev_addr)
{
  (void) dev_addr;

  filesystem_umount();

  // end block device
  msc_block_dev.end();
}
