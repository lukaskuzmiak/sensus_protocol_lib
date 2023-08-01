#ifndef _Sensus_Protocol_h_
#define _Sensus_Protocol_h_

#include <Arduino.h>

class SensusProtocol {
  int clock_pin;
  int read_pin;
  bool read_pin_pullup;

  static const auto clock_ON = HIGH;
  static const auto clock_OFF = LOW;
  
  void powerUp();
  void powerDown();
  int readBit();
  char readByte();

public:
  SensusProtocol(int clock_pin, int read_pin, bool read_pin_pullup = true);
  void setup(int reset_wait=5000);
  int getClockPin() const;
  char* readData(int max_bytes=50);
  void slowBitRead(int wait);
};


#endif //  _Sensus_Protocol_h_
