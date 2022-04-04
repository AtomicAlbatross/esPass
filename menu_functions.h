void setMenuIndicator(byte index, byte limit = MAX_MENU_ITEMS_ON_SCREEN - 1)
{
  if (index < 0) {
    index = 0;

  } else if (index > limit) {
    index = limit;
  }
  tft.setTextColor(TEXT_COLOR, BACKGROUND_COLOR);
  for (byte i = 0; i < MAX_MENU_ITEMS_ON_SCREEN; i++)
  {
    tft.setCursor(0, i * MENU_ITEM_HEIGHT, FONT);
    tft.print(" ");
  }
  tft.setCursor(0, index * MENU_ITEM_HEIGHT, FONT);
  tft.print(">");
  menuIndicatorIndex = index;
}

void displayEntries(int startValue = 0, byte page = 0)
{
  tft.fillScreen(BACKGROUND_COLOR);
  int limit = MAX_MENU_ITEMS_ON_SCREEN;
  if (columnCounter - startValue <= MAX_MENU_ITEMS_ON_SCREEN) {
    limit = columnCounter - startValue + 1;
  }
  for (int i = 0; i < limit - 1; i++)
  {

    tft.drawString(table[i + startValue][0], 20, i * MENU_ITEM_HEIGHT, FONT);
    tft.drawRightString(table[i + startValue][1], WIDTH - 20, i * MENU_ITEM_HEIGHT, FONT);

  }
}

byte chooseListAction(String msg[], byte limit = 0)
{
  if (limit == 0) limit = sizeof(msg) / sizeof(msg[0]);
  tft.fillScreen(BACKGROUND_COLOR);
  for (byte i = 0; i < limit; i++)
  {
    tft.setCursor(20, i * MENU_ITEM_HEIGHT, FONT);
    tft.print(msg[i]);
  }
  byte previousMenuIndicator = menuIndicatorIndex;
  setMenuIndicator(0);
  enter_triggered = false;
  up_triggered = false;
  down_triggered = false;

  while (!enter_triggered)
  {
    up.check();
    down.check();
    enter.check();
    if (up_triggered)
    {
      up_triggered = false;
      setMenuIndicator(menuIndicatorIndex - 1, limit);
    }
    else if (down_triggered)
    {
      down_triggered = false;
      setMenuIndicator(menuIndicatorIndex + 1, limit);
    }
    else if (enter_triggered)
    {
      enter_triggered = false;
      byte indicator = menuIndicatorIndex;
      setMenuIndicator(previousMenuIndicator);
      return indicator;


    }
  }
}



void addEntry()
{
  tft.fillScreen(BACKGROUND_COLOR);
  tft.setTextColor(TEXT_COLOR, BACKGROUND_COLOR);
  enter_triggered = false;
  printCenterUp("[SERVICE NAME]");
  tft.setCursor(0, HEIGHT / 2, FONT);
  String service_name;
  byte currentIndex = 0;
  byte xpos = 0;
  enter_long_triggered = false;
  while (!enter_long_triggered)
  {
    up.check();
    enter.check();
    down.check();
    if (up_triggered)
    {
      currentIndex = (currentIndex + 1) % sizeof(alphabet);
      tft.print(alphabet[currentIndex]);
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      up_triggered = false;
    }
    else if (enter_triggered)
    {
      service_name = service_name + alphabet[currentIndex];
      enter_triggered = false;
      xpos = xpos + 10;
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      tft.print(alphabet[0]);
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      currentIndex = 0;
    }
    else if (down_triggered)
    {
      if (currentIndex > 0)
      {
        currentIndex = (currentIndex - 1) % sizeof(alphabet);
      }
      else
      {
        currentIndex = sizeof(alphabet) - 1;
      }
      tft.print(alphabet[currentIndex]);
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      down_triggered = false;
    }
  }
  tft.fillScreen(BACKGROUND_COLOR);

  printCenterUp("[USER NAME]");
  tft.setCursor(0, HEIGHT / 2, FONT);
  String user_name;
  currentIndex = 0;
  xpos = 0;
  enter_long_triggered = false;
  while (!enter_long_triggered)
  {
    up.check();
    enter.check();
    down.check();
    if (up_triggered)
    {
      currentIndex = (currentIndex + 1) % sizeof(alphabet);
      tft.print(alphabet[currentIndex]);
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      up_triggered = false;
    }
    else if (enter_triggered)
    {
      user_name = user_name + alphabet[currentIndex];
      enter_triggered = false;
      xpos = xpos + 10;
      tft.setCursor(xpos, HEIGHT / 2, FONT);

      tft.print(alphabet[0]);
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      currentIndex = 0;
    }
    else if (down_triggered)
    {
      if (currentIndex > 0)
      {
        currentIndex = (currentIndex - 1) % sizeof(alphabet);
      }
      else
      {
        currentIndex = sizeof(alphabet) - 1;
      }
      tft.print(alphabet[currentIndex]);
      tft.setCursor(xpos, HEIGHT / 2, FONT);
      down_triggered = false;
    }
  }

  String password = "";
  currentIndex = 0;
  xpos = 0;
  enter_long_triggered = false;
  String options[] = {"TYPE PASSWORD", "GENERATE PASSWORD"};
  byte index = chooseListAction(options, 2);
  switch (index)
  {
    case 0:
      // password enter screen
      tft.fillScreen(BACKGROUND_COLOR);
      printCenterUp("[PASSWORD]");
      tft.setCursor(0, HEIGHT / 2, FONT);

      currentIndex = 0;
      xpos = 0;
      enter_long_triggered = false;
      while (!enter_long_triggered)
      {
        up.check();
        enter.check();
        down.check();
        if (up_triggered)
        {
          currentIndex = (currentIndex + 1) % sizeof(alphabet);
          tft.print(alphabet[currentIndex]);
          tft.setCursor(xpos, HEIGHT / 2, FONT);
          up_triggered = false;
        }
        else if (enter_triggered)
        {
          password = password + alphabet[currentIndex];
          enter_triggered = false;
          xpos = xpos + 10;
          tft.setCursor(xpos, HEIGHT / 2, FONT);

          tft.print(alphabet[0]);
          tft.setCursor(xpos, HEIGHT / 2, FONT);
          currentIndex = 0;
        }
        else if (down_triggered)
        {
          if (currentIndex > 0)
          {
            currentIndex = (currentIndex - 1) % sizeof(alphabet);
          }
          else
          {
            currentIndex = sizeof(alphabet) - 1;
          }
          tft.print(alphabet[currentIndex]);
          tft.setCursor(xpos, HEIGHT / 2, FONT);
          down_triggered = false;
        }
      }
      break;
    case 1:
      while (password.length() < MIN_PASSWORD_LENGTH)
      {

        password = password + wordList[random(0, WORD_LIST_SIZE - 1)];

      }
      tft.fillScreen(BACKGROUND_COLOR);
      tft.setCursor(20, HEIGHT / 2 - 50, FONT);
      printCenterUp("[GENERATED PASSWORD]");
      printCenter(password);
      enter_triggered = false;
      enter_long_triggered = false;

      while (!enter_long_triggered || !enter_triggered) {
        enter.check();
      }
      enter_triggered = false;
      enter_long_triggered = false;
      break;



    default:
      break;
  }

  file.close();
  table[columnCounter][0] = service_name;
  table[columnCounter][1] = user_name;
  table[columnCounter][2] = password;
  columnCounter = columnCounter + 1;
  writeEntries();
}

void listEntries(bool deleteMode = false)
{
  tft.fillScreen(BACKGROUND_COLOR);
  readDataFile();
  displayEntries(0);
  enter_triggered = false;
  enter_long_triggered = false;
  byte page = 0;
  setMenuIndicator(0);
  while (!enter_triggered)
  {
    up.check();
    down.check();
    enter.check();
    if (up_triggered)
    {
      up_triggered = false;
      if (menuIndicatorIndex == 0 && page > 0)
      {
        page = page - 1;

        displayEntries(page * MAX_MENU_ITEMS_ON_SCREEN);
        setMenuIndicator(MAX_MENU_ITEMS_ON_SCREEN - 1);
      }
      else
      {
        setMenuIndicator(menuIndicatorIndex - 1);
      }
    }
    else if (up_long_triggered) {
      up_long_triggered = false;

      if (page > 0) {
        page = page - 1;
        displayEntries(page * MAX_MENU_ITEMS_ON_SCREEN);
         setMenuIndicator(menuIndicatorIndex);
      }

    }
    else if (down_triggered)
    {
      down_triggered = false;
      if (menuIndicatorIndex == MAX_MENU_ITEMS_ON_SCREEN - 1)
      {
        // redraw menu
        page = page + 1;
        displayEntries(MAX_MENU_ITEMS_ON_SCREEN * page);
        setMenuIndicator(0);
      }
      else
      {
        setMenuIndicator(menuIndicatorIndex + 1);
      }
    } else if (down_long_triggered) {
      down_long_triggered = false;
      if (page * MAX_MENU_ITEMS_ON_SCREEN < columnCounter) {
      page = page + 1;
      displayEntries(MAX_MENU_ITEMS_ON_SCREEN * page);
      setMenuIndicator(menuIndicatorIndex);
      }
    }
    else if (enter_triggered)
    {
      enter_triggered = false;
      if (deleteMode)
      {
        table[menuIndicatorIndex + page * MAX_MENU_ITEMS_ON_SCREEN][0] = "";
        writeEntries();
        break;
      }
      else
      {

        // display screen to choose between bluetooth and normal Keyboard mode
        String options[] = {"USE KEYBOARD", "USE BLUETOOTH", "DISPLAY PASSWORD"};
        byte index = chooseListAction(options, 3);
        if (index == 2)
        {
          tft.fillScreen(BACKGROUND_COLOR);
          tft.drawCentreString(table[menuIndicatorIndex + page * MAX_MENU_ITEMS_ON_SCREEN][2], HEIGHT / 2, WIDTH / 2, FONT);
          enter_triggered = false;
          enter_long_triggered = false;
          while (!enter_long_triggered || !enter_triggered) {
            enter.check();
          }
          enter_triggered = false;
          enter_long_triggered = false;
        }
        else
        {

          String options[] = {"TYPE USERNAME <TAB> PASSWORD", "USERNAME ONLY", "PASSWORD ONLY"};
          byte actionIndex = chooseListAction(options, 3);
          String printString = "";
          if (actionIndex == 0)
          {
            printString = table[menuIndicatorIndex + page * MAX_MENU_ITEMS_ON_SCREEN][1] + "\t" + table[menuIndicatorIndex + page * MAX_MENU_ITEMS_ON_SCREEN][2];
          }
          else if (actionIndex == 1)
          {
            printString = table[menuIndicatorIndex + page * MAX_MENU_ITEMS_ON_SCREEN][1];
          }
          else if (actionIndex == 2)
          {
            printString = table[menuIndicatorIndex + page * MAX_MENU_ITEMS_ON_SCREEN][2];
          }
          if (index == 0)
          {
            Serial2.print(printString);
          }
          else if (index == 1)
          {
            BleKeyboard bleKeyboard("ESP32 KeySafe", "SpaceDuck");
            bleKeyboard.begin();
            unsigned long start_time = millis();
            tft.fillScreen(BACKGROUND_COLOR);

            printCenter("Waiting for connection, timeout is 10 seconds...");
            while (!bleKeyboard.isConnected() && millis() - start_time < 10000)
            {

            }
            if (bleKeyboard.isConnected())
            {

              bleKeyboard.print(printString);
              delay(100);
              bleKeyboard.releaseAll();
            }

            else
            {
              tft.fillScreen(BACKGROUND_COLOR);
              printCenter("No connection made in 10 seconds!");

              delay(1000);
            }
            bleKeyboard.end();


          }
          printString = "";
        }
      }
      break;
    }
    else if (enter_long_triggered)
    {
      enter_long_triggered = false;
      break;
    }
  }
}

void exportEntries() {

  String options[] = {"CONFIRM EXPORT"};
  byte index = chooseListAction(options, 1);

  file = SPIFFS.open("/data.txt", "r");
  String output = "";
  while (file.available()) {
    Serial2.print((char) file.read(), HEX);
  }
  file.close();

}

void deleteEntries()
{
  String options[] = {"SET NEW KEY", "KEEP OLD KEY"};
  byte index = chooseListAction(options, 2);
  Serial.println(index);
  if (index == 0) {

    printCenterUp("[ENTER NEW KEY]");
    tft.setCursor(0, HEIGHT / 2, FONT);
    tft.print(numbers[0]);
    tft.setCursor(0, HEIGHT / 2, FONT);
    byte num_chars = 0;
    byte xpos = 0;
    byte next_xpos = 0;
    byte currentIndex = 0;
    key = "";
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
    char key_buf[16];

    for (byte i = 0; i < PIN_LENGTH; i++)
    {
      key[i] = alphabet[((String)key[i]).toInt()];
    }
    String key_dup = key;
    while (key_dup.length() < PIN_UNLOCK_LENGTH) {
      key_dup += key;
    }
    key = key_dup;
  }

  emptyFile();





}

void drawMainMenu()
{
  tft.fillScreen(BACKGROUND_COLOR);
  for (byte i = 0; i < MAIN_MENU_LENGTH; i++)
  {
    tft.setCursor(20, i * MENU_ITEM_HEIGHT, FONT);
    tft.print(MainMenuItems[i]);
  }
  setMenuIndicator(0);
  enter_triggered = false;
  while (!enter_triggered)
  {
    up.check();
    down.check();
    enter.check();
    if (up_triggered)
    {
      up_triggered = false;
      setMenuIndicator(menuIndicatorIndex - 1);
    }
    else if (down_triggered)
    {
      down_triggered = false;
      setMenuIndicator(menuIndicatorIndex + 1);
    }
    else if (enter_triggered)
    {
      enter_triggered = true;
      switch (menuIndicatorIndex)
      {
        case 0:
          addEntry();
          break;
        case 1:
          listEntries(false);
          break;
        case 2:
          listEntries(true);
          break;
        case 3:
          deleteEntries();
          break;
        case 4:
          exportEntries();
          break;
      }
      break;
    }
  }
}
