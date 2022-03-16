// Set the width and the height of the display:
#define HEIGHT 240
#define WIDTH 240

// The TFT Font to use (refer to the tft_eSPI lib for more information)
#define FONT 2

// Pins to be used for the buttons:
#define ENTER_BUTTON 25
#define UP_BUTTON 26
#define DOWN_BUTTON 32

// The length of your pin (will be concatenated until length is >= 16)
// See section "Security considerations" in the included README.
#define PIN_LENGTH 4

// Minimum length of the Generated passwords. As you dont have to remember them, higher is better.
#define MIN_PASSWORD_LENGTH 25

// Your secret reset pin. See section "Sofware notes" for more details.
// Note that only the first PIN_LENGTH chars are used as the pin.
#define SECRET_RESET_KEY "1234567890"


// Background and text color. See the TFT lib documentation for available colors.

#define BACKGROUND_COLOR TFT_BLACK
#define TEXT_COLOR TFT_WHITE


//
//  ADVANCED OPTIONS (you do not need to touch this unless you know what you are doing)
//


// Control char length to verify pin correctness. Must be at least 16 chars long.
#define CONTROL_CHAR_LENGTH 16
#define CONTROL_CHAR "CONTROLCONTROLLL"

// Length of the Encryption key used. Has to be exactly 16
#define PIN_UNLOCK_LENGTH 16

// Height of each Menu Item. Changing this value has not been tested.
#define MENU_ITEM_HEIGHT 20

// Comment the next define when you dont want to use the NimBLE bluetooth stack
// although it is not recommended to do so:
#define USE_NIMBLE 
