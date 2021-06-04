#include "xTS_AdaptationField.h"

void xTS_AdaptationField::Reset()
{
	AFL=0;
	DC=0;
	RA=0;
	SP1=0;
	PR=0;
	OR=0;
	SP2=0;
	TP=0;
	EX=0;
}

int32_t xTS_AdaptationField::Parse(const uint8_t* Input, uint8_t AdaptationFieldControl)
{
	if (AdaptationFieldControl == 2 || AdaptationFieldControl == 3) {
		AFL = Input[4];
		DC = Input[5] & 0b10000000;
		RA = Input[5] & 0b01000000;
		SP1 = Input[5] & 0b00100000;
		PR = Input[5] & 0b00010000;
		OR = Input[5] & 0b00001000;
		SP2 = Input[5] & 0b00000100;
		TP = Input[5] & 0b00000010;
		EX = Input[5] & 0b00000001;
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
