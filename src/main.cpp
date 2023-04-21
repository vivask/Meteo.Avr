#include <MICS6814.h>
#include <tinySPI.h>

#define NOT_USE   (-1)

#define PIN_CO    PB3
#define PIN_NO2   PB4
#define PIN_NH3   PB5

#define PIN_MOSI  PB1
#define PIN_SCK   PB2
#define PIN_RST   PB0

volatile byte rst = 0;

MICS6814 gas(PIN_CO, PIN_NO2, PIN_NH3);

typedef struct{
  uint16_t resistance;
  uint16_t base_reistance;
  float current_ratio;
  float meashure;
}meashure_6814_t;

void (* resetFunc) (void) = 0;

ISR(INT0_vect) {
  rst = digitalRead(PIN_RST);
}

void setup() {
 
  /* SPI configure */
  SPI.begin(PIN_MOSI, NOT_USE, PIN_SCK);

  /* External Interrupt configure */
  GIMSK |= (1 << INT0); // enable external interrupt
  MCUCR |= (1 << ISC00); // CHANGE mode
  pinMode(PIN_RST, INPUT_PULLUP);

  /* MICS6814 calibrate */
  // gas.calibrate();
}

void loop() {
  meashure_6814_t data[3] = {0};

  if ( rst ) {
    resetFunc();
    delay(100);
  }

  // data[CO] = {
  //   resistance: gas.getResistance(CH_CO),
  //   base_reistance: gas.getBaseResistance(CH_CO),
  //   current_ratio: gas.getCurrentRatio(CH_CO),
  //   meashure: gas.measure(CO),
  // };
  // delay(50);

  // data[NO2] = {
  //   resistance: gas.getResistance(CH_NO2),
  //   base_reistance: gas.getBaseResistance(CH_NO2),
  //   current_ratio: gas.getCurrentRatio(CH_NO2),
  //   meashure: gas.measure(NO2),
  // };
  // delay(50);

  // data[NH3] = {
  //   resistance: gas.getResistance(CH_NH3),
  //   base_reistance: gas.getBaseResistance(CH_NH3),
  //   current_ratio: gas.getCurrentRatio(CH_NH3),
  //   meashure: gas.measure(NH3),
  // };

  uint8_t* p =  (uint8_t*)data;

  for (uint8_t i=0; i < sizeof(data); i++) {
    SPI.transfer(p[i]);
  }
  
  delay(1000);
}
