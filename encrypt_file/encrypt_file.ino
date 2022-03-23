#include "FS.h"
#include "SPIFFS.h"
#include "Cipher.h"


#define PIN_LENGTH 4
#define PIN_UNLOCK_LENGTH 16


File file;
Cipher *cipher = new Cipher();
char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  file = SPIFFS.open("/data.txt", "r");
  String buff = "";
  String block_buff = "";
  String key = "3754";
  while (file.available())
  {
    block_buff = block_buff + (char)file.read();
  }
  
  for (byte i = 0; i < PIN_LENGTH; i++)
  {
    key[i] = alphabet[((String)key[i]).toInt()];
  }
  String key_dup = key;
  while (key_dup.length() < PIN_UNLOCK_LENGTH) {
    key_dup += key;
  }
  key = key_dup;
  cipher->setKey((char *)key.c_str());
  block_buff = "CONTROLCONTROLLL" + block_buff;
  block_buff = cipher->encryptString(block_buff);
  file.close();
  file = SPIFFS.open("/data.txt", "w");
  file.print(block_buff);
  file.close();
  Serial.println("DONE!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
