#include "SensusProtocol.h"

void SensusProtocol::powerUp() {
  digitalWrite(clock_pin, clock_ON); // power on meter
  delay(500);
}

void SensusProtocol::powerDown() {
  digitalWrite(clock_pin, clock_OFF); // power on meter
}

int SensusProtocol::readBit() {
  digitalWrite(clock_pin, clock_OFF);
  delay(1);
  digitalWrite(clock_pin, clock_ON);
  delay(1); // Seems to work even without it, but just for sure
  int val = digitalRead(read_pin); // LOW is 1 and HIGH is 0
  // ESP_LOGE("SensusProtocol", "bit: %i\n", val);
  return val;
}

char SensusProtocol::readByte() {
  char result = 0;
  bool parity = false;
  if (readBit() != 0) {
    ESP_LOGE("SensusProtocol", "Start bit is not 0, error");
    return -1;
  }
  for (int i = 0; i < 7; ++i) {
    if (readBit()) {
      result |= (1 << i);
      parity = !parity;
    }
  }
  if (readBit() != parity) {
    ESP_LOGE("SensusProtocol", "Parity error");
    return -1;
  }
  if (readBit() != 1) {
    ESP_LOGE("SensusProtocol", "Stop bit is not 1, error");
    return -1;
  }
  ESP_LOGE("SensusProtocol", "byte: %i, %c\n", result, result);
  return result;    
}
  
SensusProtocol::SensusProtocol(int clock_pin, int read_pin, bool read_pin_pullup)
 : clock_pin(clock_pin), read_pin(read_pin), read_pin_pullup(read_pin_pullup) {}

void SensusProtocol::setup(int reset_wait) {
  ESP_LOGE("SensusProtocol", "setup using pins: clk: %i, read: %i, read_pullup: %i ...\n", clock_pin, read_pin, read_pin_pullup);
  pinMode(clock_pin, OUTPUT);
  digitalWrite(clock_pin, clock_OFF); // power off the meter
  auto input = INPUT;
  if (read_pin_pullup) {
    input = INPUT_PULLUP;
  }
  pinMode(read_pin, input);
  delay(reset_wait); // make sure that the meter is reset
}

int SensusProtocol::getClockPin() const { return clock_pin; }

char* SensusProtocol::readData(int max_bytes) {
  char* result = new char[max_bytes];
  powerUp();
  for (int i = 0; i < max_bytes; ++i) {
    char c = readByte();
    if (c == '\r') {
      result[i] = '\0';
      break;
    }
    result[i] = c;
  }
  powerDown();
  return result;
}

void SensusProtocol::slowBitRead(int wait) {
  powerUp();
  while (true) {
    readBit();
    delay(wait);
  }
}
