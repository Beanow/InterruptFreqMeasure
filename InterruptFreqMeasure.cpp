
#include "InterruptFreqMeasure.h"

#define INTFREQCAP_BUFFER_LEN 12
static volatile uint32_t buffer_value[INTFREQCAP_BUFFER_LEN];
static volatile uint32_t lastPulseTime;
static volatile uint8_t buffer_head, buffer_tail;
static uint8_t interruptPin, interruptNum, mode;

static void interrupt_callback(void){
  uint32_t now, period;
  
  //Measure period.
  now = micros();
  period = now - lastPulseTime;
  lastPulseTime = now;
  
  //Store it into the buffer.
  uint8_t i = buffer_head + 1;
  if (i >= INTFREQCAP_BUFFER_LEN) i = 0;
  if (i != buffer_tail) {
    buffer_value[i] = period;
    buffer_head = i;
  }
  
}

void InterruptFreqMeasureClass::begin(uint8_t ip, uint8_t in, uint8_t m){
  interruptPin=ip;
  interruptNum=in;
  mode=m;
  buffer_head=0;
  buffer_tail=0;
  pinMode(interruptPin,INPUT);
  lastPulseTime=micros();
  attachInterrupt(interruptNum,interrupt_callback,mode);
}

uint8_t InterruptFreqMeasureClass::available(void)
{
  uint8_t head, tail;

  head = buffer_head;
  tail = buffer_tail;
  if (head >= tail) return head - tail;
  return INTFREQCAP_BUFFER_LEN + head - tail;
}

uint32_t InterruptFreqMeasureClass::read(void)
{
  uint8_t head, tail;
  uint32_t value;

  head = buffer_head;
  tail = buffer_tail;
  if (head == tail) return 0xFFFFFFFF;
  tail = tail + 1;
  if (tail >= INTFREQCAP_BUFFER_LEN) tail = 0;
  value = buffer_value[tail];
  buffer_tail = tail;
  return value;
}

void InterruptFreqMeasureClass::end(){
  detachInterrupt(interruptNum);
}

InterruptFreqMeasureClass InterruptFreqMeasure;
