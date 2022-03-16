void writeEntries()
{
  file.close();
  file = SPIFFS.open("/data.txt", "w");
  String buff = CONTROL_CHAR;
  for (byte i = 0; i < columnCounter; i++)
  {
    if (table[i][0] != "")
    {
      buff = buff + table[i][0] + "|" + table[i][1] + "|" + table[i][2] + ",";
    }
  }
  cipher->setKey((char *)key.c_str());
  file.print(cipher->encryptString(buff));
  Serial.println(buff);
  file.close();
}




void emptyFile()  {
  file.close();
  file = SPIFFS.open("/data.txt", "w");
  cipher->setKey((char *)key.c_str());
  file.print(cipher->encryptString(CONTROL_CHAR));
  file.close();

}


void loading() {

  tft.fillScreen(BACKGROUND_COLOR);
  tft.drawCentreString("[LOADING]", HEIGHT / 2, WIDTH / 2, FONT);

}

void printCenter(String msg) {

  tft.fillScreen(BACKGROUND_COLOR);
  tft.drawCentreString(msg, HEIGHT / 2, WIDTH / 2, FONT);

}

void printCenterUp(String msg) {

  tft.fillScreen(BACKGROUND_COLOR);
  tft.drawCentreString(msg, HEIGHT / 2, 20, FONT);

}


void readDataFile()
{

  file.close();
  file = SPIFFS.open("/data.txt", "r");
  String buff = "";
  String block_buff = "";
  char previous_char;
  columnCounter = 0;
  rowCounter = 0;
  loading();
  while (file.available())
  {
    block_buff = block_buff + (char)file.read();
  }
  cipher->setKey((char *)key.c_str());
  block_buff = cipher->decryptString(block_buff);
  if (block_buff.substring(0,CONTROL_CHAR_LENGTH) != CONTROL_CHAR)
  {
    printCenter("Wrong Key!");
    enter_triggered = false;
    while (!enter_triggered)
    {
      enter.check();
      if (enter_triggered) {
        enter_triggered = false;
        ESP.restart();
      }
    };
  }

  // iterate through the String block_buff and use the seperators | to distinguish between fields and , to separate rows
  for (int i = CONTROL_CHAR_LENGTH; i < block_buff.length(); i++)
  {
    char dat = block_buff[i];
    if (dat == '|')
    {
      table[columnCounter][rowCounter] = buff;
      buff = "";
      rowCounter = rowCounter + 1;
    }
    else if (dat == ',' && previous_char != ',')
    {
      table[columnCounter][rowCounter] = buff;
      columnCounter = columnCounter + 1;
      buff = "";
      rowCounter = 0;
    }
    else
    {
      buff += dat;
    }
    previous_char = dat;

  }
  block_buff = "";
  file.close();
}
