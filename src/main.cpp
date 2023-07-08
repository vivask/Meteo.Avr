#include <Wire.h>
#include <SPI.h>
#include "MICS6814.h"

#define PIN_NO2   16
#define PIN_NH3   17
#define PIN_CO    15

#define PIN_INT   3
#define PIN_RST   4

#define STABLE_TIME 20 //µs
#define RESET_COUNT 11

volatile unsigned long previousTime;
volatile unsigned long pulse_counter = 0;

MICS6814 gas(PIN_CO, PIN_NO2, PIN_NH3);

static void reset() {
  // set low reset pin
  digitalWrite(PIN_RST, LOW);
  delay(100);
}

static void interrupt_handler() {
  unsigned long currentTime = micros();
  if (currentTime - previousTime >= STABLE_TIME) pulse_counter++;
  previousTime = currentTime;  
  if (pulse_counter == RESET_COUNT) {
    reset();
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(1);
    yield();
  };

  // RST configure
  pinMode(PIN_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_INT), interrupt_handler, FALLING); 
  digitalWrite(PIN_RST, HIGH); 
  pinMode(PIN_RST, OUTPUT);

  // SPI configure
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8); //divide the clock by 8

  // MICS6814 calibrate
  gas.calibrate();

}

void loop() {

  // reset rst pulse counter
  pulse_counter = 0;

  mics6814_data_t* data = gas.measure();

  Serial.print("CO: ");
  Serial.println(data->co);
  Serial.print("NO2: ");
  Serial.println(data->no2);
  Serial.print("NH3: ");
  Serial.println(data->nh3);
  Serial.print("Status: ");
  Serial.println(data->status);

  // check calibration result
  if (data->status == MICS6814_FAIL) {
    // calibration fail
    reset();
  }

  gas.spiTransfer(data);

  delay(1000);
}

