/*
 * I2C command processor.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <Wire.h>

#define SLAVE_ADDRESS 0x6B
#define TIME_OUT 100

#define STATE_IDLE 0
#define STATE_RD   1
#define STATE_WR   2

#define STATE_MAX 2

static char buffer[250];
static volatile size_t buf_idx = 0;

static volatile int state = 0;
static volatile unsigned long state_set_at = 0;

void setup()
{
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.begin(115200);
  while(!Serial);

  Serial.println("Starting..");
  memset((void*)buffer, 0, sizeof(buffer));
}

void loop()
{
  if(state_set_at + TIME_OUT < millis()) {
    if(state == STATE_WR) {
      for(int idx = 0; idx < 250; idx++) {
        Serial.print(buffer[idx]);
      }
  
      Serial.println('.');
      memset((void*)buffer, 0, sizeof(buffer));
    }

    state = STATE_IDLE;
    buf_idx = 0;
  }

  delay(10);
}

static void requestEvent(void)
{
  unsigned char x = 0xAB;

  Serial.println("Read event!");

  Wire.write(x++);
  Wire.write(x++);
  Wire.write(x);

  state = STATE_IDLE;
}

static void receiveEvent(int howMany)
{
  switch(state) {
  default:
    Serial.println("Unknown state..");
    
    while(Wire.available()) {
      Wire.read();
    }
    break;

  case STATE_IDLE:
    state = Wire.read();
    Serial.print("State set.. ");
    Serial.println(state);
    buf_idx = 0;
    state_set_at = millis();

    if(state > STATE_MAX || state < 0) {
      state = STATE_IDLE;

      while(Wire.available()) {
        Wire.read();
      }
      break;
    }
    break;

  case STATE_WR:
    while(Wire.available()) {
      char c = Wire.read();

      if(buf_idx < 250) {
        buffer[buf_idx] = c;
        buf_idx ++;
      }
    }
    
    break;
  }
}

