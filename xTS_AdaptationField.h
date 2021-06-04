#pragma once
#include "tsCommon.h"
#include <string>

class xTS_AdaptationField
{
protected:
	//AF length
	//mandatory fields
	uint8_t AFL;
	bool DC;
	bool RA;
	bool SP1;
	bool PR;
	bool OR;
	bool SP2;
	bool TP;
	bool EX;
	uint64_t PCR;
	uint16_t PCRE;
	uint64_t OPCR;
	uint16_t OPCRE;
public:
	void Reset();
	int32_t Parse(const uint8_t * Input, uint8_t AdaptationFieldControl);
	void Print() const;
public:
	//derrived values
	uint32_t getNumBytes() const;
};