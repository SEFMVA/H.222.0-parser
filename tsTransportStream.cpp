#include "tsTransportStream.h"

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================

void     xTS_PacketHeader::Reset() {
    SB = 0;
    E = 0;
    S = 0;
    P = 0;
    PID = 0;
    TSC = 0;
    AFC = 0;
    CC = 0;

}
int32_t  xTS_PacketHeader::Parse(const uint8_t* Input) {
    SB = Input[0];
    E = (Input[1] & 0b10000000)>>7;
    S = (Input[1] & 0b01000000)>>6;
    P = (Input[1] & 0b00100000)>>5;
    PID = ((Input[1] & 0b00011111)<<8)+Input[2];
    TSC = (Input[3] & 0b11000000)>>6;
    AFC = (Input[3] & 0b00110000)>>4;
    CC  = Input[3] & 0b00001111;
    return 1;
}
void     xTS_PacketHeader::Print() const {
    printf("TS: SB=%d E=%d S=%d P=%d PID=%d TSC=%d AF=%d CC=%d", SB, E, S, P, PID, TSC, AFC, CC);
}

uint8_t xTS_PacketHeader::getAdaptationFieldControl() const
{
    return AFC;
}

uint16_t xTS_PacketHeader::getPID() const
{
    return PID;
}

uint8_t xTS_PacketHeader::getSyncByte() const
{
    return SB;
}

uint8_t xTS_PacketHeader::getContinuityCounter() const
{
    return CC;
}

uint8_t xTS_PacketHeader::getStartIndicator() const
{
    return S;
}

bool xTS_PacketHeader::hasAdaptationField()const
{
    if (AFC == 2 || AFC == 3)return true;
    return false;
}




//=============================================================================================================================================================================
