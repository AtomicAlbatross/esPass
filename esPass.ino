#include "FS.h"
#include "SPIFFS.h"
#include "Cipher.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <AceButton.h>
#include <String>
#include "Words.h"
#include "config.h"


#include <BleKeyboard.h>

#define WORD_LIST_SIZE sizeof(wordList) / sizeof(wordList[0])
#define MAX_MENU_ITEMS_ON_SCREEN HEIGHT / MENU_ITEM_HEIGHT
#define MAIN_MENU_LENGTH sizeof(MainMenuItems) / sizeof(MainMenuItems[0])

using namespace ace_button;

TFT_eSPI tft;
ButtonConfig upConfig;
ButtonConfig downConfig;
ButtonConfig enterConfig;
AceButton up(&upConfig);
AceButton down(&downConfig);
AceButton enter(&enterConfig);
File file;



byte numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
bool up_triggered;
bool down_triggered;
bool enter_triggered;
bool enter_long_triggered;
bool up_long_triggered;
bool down_long_triggered;
byte menuIndicatorIndex = 0;

String MainMenuItems[5] = {"ADD", "LIST", "DELETE ENTRY", "DELETE ALL ENTRIES", "EXPORT ALL ENTRIES"};
Cipher *cipher = new Cipher();
String table[100][3];
String key;
byte rowCounter = 0;
byte columnCounter = 0;

#include "fs_functions.h"
#include "menu_functions.h"


void handleEnterEvent(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  if (eventType == AceButton::kEventReleased)
  {
    enter_triggered = true;
  }
  else if (eventType == AceButton::kEventLongPressed)
  {
    enter_long_triggered = true;
  }
}
void handleUpEvent(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  if (eventType == AceButton::kEventReleased)
  {
    up_triggered = true;
  } else if (eventType == AceButton::kEventLongPressed)
  {
    up_long_triggered = true;
  }
}
void handleDownEvent(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  if (eventType == AceButton::kEventReleased)
  {
    down_triggered = true;
  } else if (eventType == AceButton::kEventLongPressed)
  {
    down_long_triggered = true;
  }
}


void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  tft.init();
  tft.fillScreen(BACKGROUND_COLOR);
  randomSeed(analogRead(A0));
  pinMode(ENTER_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  up.init(UP_BUTTON);
  down.init(DOWN_BUTTON);
  enter.init(ENTER_BUTTON);
  // Button setup
  enterConfig.resetFeatures();
  enterConfig.setFeature(ButtonConfig::kFeatureLongPress);
  enterConfig.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
  enterConfig.setFeature(ButtonConfig::kFeatureClick);
  enterConfig.setEventHandler(handleEnterEvent);
  upConfig.resetFeatures();
  upConfig.setFeature(ButtonConfig::kFeatureClick);
  upConfig.setFeature(ButtonConfig::kFeatureLongPress);
  upConfig.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
  upConfig.setEventHandler(handleUpEvent);
  downConfig.resetFeatures();
  downConfig.setFeature(ButtonConfig::kFeatureClick);
  downConfig.setFeature(ButtonConfig::kFeatureLongPress);
  downConfig.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
  downConfig.setEventHandler(handleDownEvent);

  // General setup of the colorscheme we will use, change if desired
    tft.setTextColor(TEXT_COLOR, BACKGROUND_COLOR);
  tft.setCursor(0, HEIGHT / 2, FONT);

  bool first_time_setup = true;
  if (!SPIFFS.begin())
  {
    printCenter("Error while mounting FileSystem!");
    while (1)
      ;
  }
   if (SPIFFS.exists("/data.txt")) {
    first_time_setup = false;
  }
 
  //Draw a simple Password screen
 if (first_time_setup) printCenterUp("[FIRST TIME SETUP]");
  tft.setCursor(0, HEIGHT / 2, FONT);
  tft.print(numbers[0]);
  tft.setCursor(0, HEIGHT / 2, FONT);
  byte num_chars = 0;
  byte xpos = 0;
  byte next_xpos = 0;
  byte currentIndex = 0;
  while (num_chars < PIN_LENGTH)
  {
    up.check();
    enter.check();
    down.check();
    if (up_triggered)
    {
      currentIndex = (currentIndex + 1) % sizeof(numbers);
      tft.print(numbers[currentIndex]);
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      up_triggered = false;
    }
    else if (enter_triggered)
    {
      key = key + numbers[currentIndex];
      enter_triggered = false;
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      tft.print("*");
      xpos = xpos + 10;
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      tft.print(numbers[0]);
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      num_chars = num_chars + 1;

      currentIndex = 0;
    }
    else if (down_triggered)
    {
      if (currentIndex > 0)
      {
        currentIndex = (currentIndex - 1) % sizeof(numbers);
      }
      else
      {
        currentIndex = sizeof(numbers) - 1;
      }
      tft.print(numbers[currentIndex]);
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      down_triggered = false;
    }
  }

 String reset_key = (String) SECRET_RESET_KEY;
  
  for (byte i = 0; i < PIN_LENGTH; i++)
  {
    key[i] = alphabet[((String)key[i]).toInt()];
    reset_key[i] = alphabet[((String)SECRET_RESET_KEY[i]).toInt()];
  }

  if (key == reset_key.substring(0, PIN_LENGTH)) {
    file.close();
   SPIFFS.remove("/data.txt");
   ESP.restart();
  }
  String key_dup = key;
  while (key_dup.length() < PIN_UNLOCK_LENGTH) {
    key_dup += key;
  }
  key = key_dup;
  cipher->setKey((char *)key.c_str());
  
 
  
  if (first_time_setup) {
  emptyFile();
  }
  first_time_setup = false;
  
  
  readDataFile();
  file.close();
}
void loop()
{
  drawMainMenu();
}
