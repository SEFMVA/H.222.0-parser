#include "xTS_AdaptationField.h"

void xTS_AdaptationField::Reset()
{
	AFL = 0;
	DC = 0;
	RA = 0;
	SP1 = 0;
	PR = 0;
	OR = 0;
	SP2 = 0;
	TP = 0;
	EX = 0;
	PCR = 0;
	PCRE = 0;
	OPCR = 0;
	OPCRE = 0;
}

int32_t xTS_AdaptationField::Parse(const uint8_t* Input, uint8_t AdaptationFieldControl)
{
	if (AdaptationFieldControl == 2 || AdaptationFieldControl == 3) {
		AFL = Input[4];
		if (AFL!=0){
			DC = Input[5] & 0b10000000;
			RA = Input[5] & 0b01000000;
			SP1 = Input[5] & 0b00100000;
			PR = Input[5] & 0b00010000;
			OR = Input[5] & 0b00001000;
			SP2 = Input[5] & 0b00000100;
			TP = Input[5] & 0b00000010;
			EX = Input[5] & 0b00000001;

			uint8_t offset = 0;
			if (PR) { 
				offset += 6;
				PCR = ((uint64_t)Input[6] << 25) + ((uint64_t)Input[7] << 17) + ((uint64_t)Input[8] << 9) + ((uint64_t)Input[9] << 1) + (((uint64_t)Input[10] >> 7) & 0b00000001);
				PCRE = ((Input[10] & 0b00000001) << 8) + Input[11];
			}
			if (OR) {
				OPCR = ((uint64_t)Input[6 + offset] << 25) + ((uint64_t)Input[7 + offset] << 17) + ((uint64_t)Input[8 + offset] << 9) + ((uint64_t)Input[9 + offset] << 1) + (((uint64_t)Input[10 + offset] >> 7) & 0b00000001);
				OPCRE = ((Input[10 + offset] & 0b00000001) << 8) + Input[11 + offset];
			}
		}else{
			DC = 0;
			RA = 0;
			SP1 = 0;
			PR = 0;
			OR = 0;
			SP2 = 0;
			TP = 0;
			EX = 0;
			PCR = 0;
			PCRE = 0;
			OPCR = 0;
			OPCRE = 0;
		}
		
	}
	else return 0;
    return 1;
}

void xTS_AdaptationField::Print() const
{
	printf("AF: L=%d DC=%d RA=%d SP=%d PR=%d OR=%d SP=%d TP=%d EX=%d", AFL, DC, RA, SP1, PR, OR, SP2, TP, EX);
}

uint32_t xTS_AdaptationField::getNumBytes() const
{
	return 1+AFL;
}
