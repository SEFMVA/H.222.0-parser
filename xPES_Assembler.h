#pragma once
#include "tsCommon.h"
#include <string>
#include "xPES_PacketHeader.h"
#include "tsTransportStream.h"
#include "xTS_AdaptationField.h"
#include <vector>

class xPES_Assembler
{
public:
	enum class eResult : int32_t
	{
		UnexpectedPID = 1,
		StreamPackedLost,
		AssemblingStarted,
		AssemblingContinue,
		AssemblingFinished,
	};
protected:
	//setup
	uint32_t m_PID;
	//buffer
	std::vector<uint8_t> m_Buffer;
	uint32_t m_BufferSize;
	uint32_t m_DataOffset;
	//operation
	int8_t m_LastContinuityCounter;
	bool m_Started;
	bool m_LookingForNextStart;
	xPES_PacketHeader m_PESH;
public:
	void Init(uint32_t PID);
	eResult AbsorbPacket(const uint8_t* TransportStreamPacket, const xTS_PacketHeader* PacketHeader, const xTS_AdaptationField* AdaptationField);

	void PrintPESH() const { m_PESH.Print(); }
	std::vector<uint8_t> getPacket() { return m_Buffer; }
	int32_t getNumPacketBytes() const { return m_BufferSize; }
	int32_t getHeaderSize() const { return m_DataOffset; }
protected:
	void xBufferReset();
	void xBufferAppend(const uint8_t* Data, int32_t Size);
private:
	void incrementContinuityCounter() { m_LastContinuityCounter = (m_LastContinuityCounter + 1) & 0b00001111; }
	void incrementContinuityCounter(uint8_t cc) { m_LastContinuityCounter = cc; }
	bool saveToFile(std::string fileName);
};