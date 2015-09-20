
#ifndef InterruptFreqMeasure_h
#define InterruptFreqMeasure_h

#include <Arduino.h>

class InterruptFreqMeasureClass {
  
  public:
    static void begin(uint8_t ip=2, uint8_t in=0, uint8_t m=RISING);
    static uint8_t available(void);
    static uint32_t read(void);
    static void end(void);
  
};

extern InterruptFreqMeasureClass InterruptFreqMeasure;

#endif
