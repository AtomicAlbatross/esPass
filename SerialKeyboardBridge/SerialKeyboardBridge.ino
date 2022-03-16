#include "Keyboard.h"

void setup() {
  Serial1.begin(9600);
  Keyboard.begin();
}

void loop() {
  if (Serial1.available() > 0) {
    Keyboard.write(Serial1.read());
  }
}
