#include <MICS6814.h>

#define PIN_CO  0
#define PIN_NO2 1
#define PIN_NH3 2

MICS6814 gas(PIN_CO, PIN_NO2, PIN_NH3);

typedef struct{
  uint16_t resistance;
  uint16_t base_reistance;
  float current_ratio;
  float meashure;
}meashure_6814_t;

int incomingByte = 0; // for incoming serial data

void setup() {
  Serial.begin(9600);
  Serial.println("Calibrate...");
  gas.calibrate();
  Serial.println("Calibrate success");
}

void loop() {
  meashure_6814_t data[3];

  data[CO] = {
    resistance: gas.getResistance(CH_CO),
    base_reistance: gas.getBaseResistance(CH_CO),
    current_ratio: gas.getCurrentRatio(CH_CO),
    meashure: gas.measure(CO),
  };
  delay(50);

  data[NO2] = {
    resistance: gas.getResistance(CH_NO2),
    base_reistance: gas.getBaseResistance(CH_NO2),
    current_ratio: gas.getCurrentRatio(CH_NO2),
    meashure: gas.measure(NO2),
  };
  delay(50);

  data[NH3] = {
    resistance: gas.getResistance(CH_NH3),
    base_reistance: gas.getBaseResistance(CH_NH3),
    current_ratio: gas.getCurrentRatio(CH_NH3),
    meashure: gas.measure(NH3),
  };

  /*Serial.print("CO: ");
  Serial.println(data[CO].meashure);
  Serial.print("NO2: ");
  Serial.println(data[NO2].meashure);
  Serial.print("NH3: ");
  Serial.println(data[NH3].meashure);*/
  Serial.write((byte*)data, sizeof(data));

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }

  delay(1000);
}
