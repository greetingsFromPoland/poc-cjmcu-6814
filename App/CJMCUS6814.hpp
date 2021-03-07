#ifndef MICS6814_H
#define MICS6814_H

#include "Arduino.h"

typedef enum
{
	CO,
	NO2,
	NH3
} gas_t;

class CJMCUS6814
{
public:
	CJMCUS6814(int pinCO, int pinNO2, int pinNH3);

	void calibrate();
	byte measure(gas_t gas);
	void print(gas_t gas);

private:
	uint8_t _pinCO;
	uint8_t _pinNO2;
	uint8_t _pinNH3;

	uint16_t _maxNH3;
	uint16_t _maxCO;
	uint16_t _maxNO2;
};

#endif