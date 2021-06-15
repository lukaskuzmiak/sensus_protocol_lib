#ifndef _Sensus_Protocol_h_
#define _Sensus_Protocol_h_

#include <Arduino.h>
#include <String>

class SensusProtocol {
  int clock_pin;
  int read_pin;
  bool read_pin_pullup;

  static const auto clock_ON = LOW;
  static const auto clock_OFF = HIGH;
  
  void powerUp();
  void powerDown();
  int readBit();
  char readByte();

public:
  SensusProtocol(int clock_pin, int read_pin, bool read_pin_pullup = true);
  void setup();
  int getClockPin() const;
  String readData();
  void slowBitRead();
};


#endif //  _Sensus_Protocol_h_
