/*
 * I2C command processor.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <Wire.h>

#define SLAVE_ADDRESS 0x6B
#define TIME_OUT 50

#define STATE_IDLE 0
#define STATE_RD   1
#define STATE_WR   2

#define STATE_MAX 2

static char buffer[250];
static volatile bool wr_update = false;
static volatile size_t buf_idx = 0;

static volatile int state = 0;
static volatile long state_set_at = 0;

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
  if(state_set_at + TIME_OUT <= millis()) {
    if(wr_update) {
      for(int idx = 0; idx < 250; idx++) {
        Serial.print(buffer[idx]);
      }
  
      Serial.println('.');
      wr_update = false;
      memset((void*)buffer, 0, sizeof(buffer));
    }

    state = STATE_IDLE;
    buf_idx = 0;
  }

  delay(5);
}

static unsigned char data[] = {0xAB, 0xAC, 0xAD};
static void requestEvent(void)
{
  if(state != STATE_RD) {
    Serial.println("Invalid read attempted!");
    return;
  }
  
  Wire.write(data, 3);
  state = STATE_IDLE;
}

static void receiveEvent(int howMany)
{
  long now = millis();

  switch(state) {
  default:
    Serial.print("Unknown state..: ");
    Serial.println(state);

    Serial.print("NOW - TMO = ");
    Serial.println(now - state_set_at);

    state = STATE_IDLE;
    while(Wire.available()) {
      Wire.read();
    }
    break;

  case STATE_IDLE:
    state = Wire.read();
    //Serial.print("State set.. ");
    //Serial.println(state);
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
    wr_update = true;
    
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
