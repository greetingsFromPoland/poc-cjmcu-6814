#include "CJMCUS6814.hpp"

CJMCUS6814 ::CJMCUS6814(int pinCO, int pinNO2, int pinNH3)
{
	_pinCO = pinCO;
	_pinNO2 = pinNO2;
	_pinNH3 = pinNH3;
}

void CJMCUS6814 ::calibrate()
{
	// The number of seconds that must pass before
	// than we will assume that the calibration is complete
	// (Less than 64 seconds to avoid overflow)
	uint8_t seconds = 10;

	// Tolerance for the average of the current value
	uint8_t delta = 3;

	// Measurement buffers
	uint16_t bufferNH3[seconds];
	uint16_t bufferCO[seconds];
	uint16_t bufferNO2[seconds];

	// Pointers for the next item in the buffer
	uint8_t pntrNH3 = 0;
	uint8_t pntrCO = 0;
	uint8_t pntrNO2 = 0;

	// The current floating amount in the buffer
	uint16_t fltSumNH3 = 0;
	uint16_t fltSumCO = 0;
	uint16_t fltSumNO2 = 0;

	// Current measurement
	uint16_t curNH3;
	uint16_t curCO;
	uint16_t curNO2;

	// Flag of stability of indications
	bool isStableNH3 = false;
	bool isStableCO = false;
	bool isStableNO2 = false;

	// We kill the buffer with zeros
	for (int i = 0; i < seconds; ++i)
	{
		bufferNH3[i] = 0;
		bufferCO[i] = 0;
		bufferNO2[i] = 0;
	}

	// Calibrate
	do
	{
		delay(1000);

		unsigned long rs = 0;

		delay(50);
		int readBuffor = 0;
		for (int i = 0; i < 3; i++)
		{
			delay(1);
			rs += analogRead(_pinNH3);
		}

		curNH3 = rs / 3;
		rs = 0;

		delay(50);
		for (int i = 0; i < 3; i++)
		{
			delay(1);
			rs += analogRead(_pinCO);
		}

		curCO = rs / 3;
		rs = 0;

		delay(50);
		for (int i = 0; i < 3; i++)
		{
			delay(1);
			rs += analogRead(_pinNO2);
		}

		curNO2 = rs / 3;

		// Update the floating amount by subtracting the value,
		// to be overwritten, and adding a new value.
		fltSumNH3 = fltSumNH3 + curNH3 - bufferNH3[pntrNH3];
		fltSumCO = fltSumCO + curCO - bufferCO[pntrCO];
		fltSumNO2 = fltSumNO2 + curNO2 - bufferNO2[pntrNO2];

		// Store d buffer new values
		bufferNH3[pntrNH3] = curNH3;
		bufferCO[pntrCO] = curCO;
		bufferNO2[pntrNO2] = curNO2;

		// Define flag states
		isStableNH3 = abs(fltSumNH3 / seconds - curNH3) < delta;
		isStableCO = abs(fltSumCO / seconds - curCO) < delta;
		isStableNO2 = abs(fltSumNO2 / seconds - curNO2) < delta;

		// Pointer to a buffer
		pntrNH3 = (pntrNH3 + 1) % seconds;
		pntrCO = (pntrCO + 1) % seconds;
		pntrNO2 = (pntrNO2 + 1) % seconds;
	} while (!isStableNH3 || !isStableCO || !isStableNO2);

	_maxNH3 = fltSumNH3 / seconds;
	_maxCO = fltSumCO / seconds;
	_maxNO2 = fltSumNO2 / seconds;

	Serial.print("_maxNH3: ");
	Serial.print(_maxNH3);
	Serial.print(" _maxCO: ");
	Serial.print(_maxCO);
	Serial.print(" _maxNO2: ");
	Serial.println(_maxNO2);
}

byte CJMCUS6814 ::measure(gas_t gas)
{
	uint8_t readedAnalogValue;

	switch (gas)
	{
	case CO:
		readedAnalogValue = analogRead(_pinCO);
		if (readedAnalogValue > _maxCO)
		{
			_maxCO = readedAnalogValue;
		}
		return map(readedAnalogValue, 0, _maxCO, 100, 0);
	case NO2 : readedAnalogValue = analogRead(_pinNO2);
		if (readedAnalogValue > _maxCO)
		{
			_maxNO2 = readedAnalogValue;
		}
		return map(readedAnalogValue, 0, _maxNO2, 100, 0);
	case NH3:
		readedAnalogValue = analogRead(_pinNH3);
		if (readedAnalogValue > _maxNH3)
		{
			_maxNH3 = readedAnalogValue;
		}
		return map(readedAnalogValue, 0, _maxNH3, 100, 0);
	}

	return 0;
}
void CJMCUS6814 ::print(gas_t gas)
{
	uint16_t readedAnalogValue;
	uint8_t mapedValued;

	switch (gas)
	{
	case CO:
		readedAnalogValue = analogRead(_pinCO);
		if (readedAnalogValue > _maxCO)
		{
			_maxCO = readedAnalogValue;
		}
		mapedValued = map(readedAnalogValue, 0, _maxCO, 100, 0);

		Serial.print("CO ");
		Serial.print("r:");
		Serial.print(readedAnalogValue);
		Serial.print(" m:");
		Serial.print(_maxCO);
		Serial.print(" map:");
		Serial.println(mapedValued);
		break;
	case NO2:
		readedAnalogValue = analogRead(_pinNO2);
		if (readedAnalogValue > _maxCO)
		{
			_maxNO2 = readedAnalogValue;
		}
		mapedValued = map(readedAnalogValue, 0, _maxNO2, 100, 0);

		Serial.print("NO2 ");
		Serial.print("r:");
		Serial.print(readedAnalogValue);
		Serial.print(" m:");
		Serial.print(_maxNO2);
		Serial.print(" map:");
		Serial.println(mapedValued);
		break;
	case NH3:
		readedAnalogValue = analogRead(_pinNH3);
		if (readedAnalogValue > _maxNH3)
		{
			_maxNH3 = readedAnalogValue;
		}
		mapedValued = map(readedAnalogValue, 0, _maxNH3, 100, 0);

		Serial.print("NH3 ");
		Serial.print("r:");
		Serial.print(readedAnalogValue);
		Serial.print(" m:");
		Serial.print(_maxNO2);
		Serial.print(" map:");
		Serial.println(mapedValued);
		break;
	}
}
