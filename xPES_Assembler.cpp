#include "xPES_Assembler.h"

void xPES_Assembler::Init(int32_t PID)
{
	m_PID = PID;
	m_LastContinuityCounter = 0;
	m_Started = false;
}

xPES_Assembler::eResult xPES_Assembler::AbsorbPacket(const uint8_t* TransportStreamPacket, const xTS_PacketHeader* PacketHeader, const xTS_AdaptationField* AdaptationField)
{
	if (PacketHeader->getPID() != m_PID)return eResult::UnexpectedPID; 
	if (m_Started && PacketHeader->getContinuityCounter() != ((m_LastContinuityCounter + 1) & 0b00001111)) {
		incrementContinuityCounter(PacketHeader->getContinuityCounter());
		return eResult::StreamPackedLost;
	}

	uint8_t offset = 4;
	if (PacketHeader->hasAdaptationField()) offset += AdaptationField->getNumBytes();
	uint8_t size = 188 - offset;

	if (size == 0) { 
		incrementContinuityCounter(PacketHeader->getContinuityCounter());
		return eResult::AssemblingContinue; 
	}

	//uint8_t *PES= new uint8_t[size];
	//std::copy_n(TransportStreamPacket+offset,size,PES);
	const uint8_t* PES = TransportStreamPacket + offset;

	if (PacketHeader->getStartIndicator())
	{
		xBufferReset();
		m_PESH.Reset();
		uint32_t headerLength=m_PESH.Parse(PES);
		m_DataOffset = headerLength;
	}

	
	uint16_t packetLength = m_PESH.getPacketLength();
	xBufferAppend(PES, size);

	if (packetLength != 0) {
		if (packetLength <= m_BufferSize)
		{
			//m_LastContinuityCounter = 0; //TODO czy zawsze sie resetuje?
			incrementContinuityCounter(PacketHeader->getContinuityCounter());
			m_Started = false;
			
			return eResult::AssemblingFinished;
		}
			
		}
	else {
		printf("TODO when length is 0");
	}

	

	if (m_Started == false && PacketHeader->getStartIndicator()) {
		incrementContinuityCounter(PacketHeader->getContinuityCounter());
		m_Started = true;
		return eResult::AssemblingStarted;
	}
	else
	{
		incrementContinuityCounter(PacketHeader->getContinuityCounter());
		return eResult::AssemblingContinue;


	}
	
}

void xPES_Assembler::xBufferReset()
{
	m_Buffer.clear();
	m_BufferSize = 0;
	m_DataOffset = 0;
}

void xPES_Assembler::xBufferAppend(const uint8_t* Data, int32_t Size)
{
	m_Buffer.insert(m_Buffer.end(), Data, Data + Size);
	m_BufferSize += Size;
}
