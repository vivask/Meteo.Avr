#ifndef MICS6814_H
#define MICS6814_H

#include "Arduino.h"

#define MICS6814_OK			(0)
#define MICS6814_FAIL		(-1)
#define MICS6814_CAL		(1)

typedef enum
{
	CH_CO,
	CH_NO2,
	CH_NH3
} channel_t;

typedef enum
{
	CO,
	NO2,
	NH3
} gas_t;

typedef struct 
{
	float		co;
	float		nh3;
	float		no2;
	int8_t 		status;
} mics6814_data_t;

class MICS6814
{
public:
	MICS6814(int pinCO, int pinNO2, int pinNH3);

	void calibrate();
	void loadCalibrationData(
		uint16_t base_NH3,
		uint16_t base_RED,
		uint16_t base_OX);

	float measure(gas_t gas);

	mics6814_data_t* measure();

	uint16_t getResistance    (channel_t channel) const;
	uint16_t getBaseResistance(channel_t channel) const;
	float    getCurrentRatio  (channel_t channel) const;
	void spiTransfer(mics6814_data_t* data);
private:
	uint8_t _pinCO;
	uint8_t _pinNO2;
	uint8_t _pinNH3;
	uint8_t _pinLED;

	uint16_t _baseNH3;
	uint16_t _baseCO;
	uint16_t _baseNO2;

	mics6814_data_t _data;
};

#endif