#include "xPES_PacketHeader.h"

void xPES_PacketHeader::Reset()
{
	m_PacketStartCodePrefix = 0;
	m_StreamId = 0;
	m_PacketLength = 0;
	m_PES_extension_flag = false;
	m_PES_header_data_length = 0;
	PTS = 0;
	DTS = 0;
}

uint32_t xPES_PacketHeader::Parse(const uint8_t* Input)
{
	uint32_t totalHeaderLength = 6;
	m_PacketStartCodePrefix = (Input[0]<< 16) + (Input[1]<<8) + Input[2];
	m_StreamId =Input[3];
	m_PacketLength = (Input[4] << 8) + Input[5];

	if (m_StreamId != eStreamId::eStreamId_program_stream_map
		&& m_StreamId != eStreamId::eStreamId_padding_stream
		&& m_StreamId != eStreamId::eStreamId_private_stream_2
		&& m_StreamId != eStreamId::eStreamId_ECM
		&& m_StreamId != eStreamId::eStreamId_EMM
		&& m_StreamId != eStreamId::eStreamId_program_stream_directory
		&& m_StreamId != eStreamId::eStreamId_DSMCC_stream
		&& m_StreamId != eStreamId::eStreamId_ITUT_H222_1_type_E){ 
	
		totalHeaderLength += 3;
		uint8_t PTS_DTS_flag = Input[7] & 0b11000000;
		bool ESCR_flag = Input[7] & 0b00100000;
		m_PES_extension_flag = Input[7] & 0b00000001;
		if (m_PES_extension_flag){
			m_PES_header_data_length = Input[8];
		}
		else {
			m_PES_header_data_length = 0;
		}
		if (PTS_DTS_flag == 128)	{
			totalHeaderLength += 5;
			if ((Input[9] & 0b11110000) != 0b00100000)
			{
				printf("PTS has wrong prefix!");
			}
			PTS = ((((uint64_t)Input[9] >> 1) & 0b00000111) << 30) + ((uint64_t)Input[10] << 22 ) + ((((uint64_t)Input[11] >> 1) & 0b01111111) << 15) + (((uint64_t)Input[12] << 7)) + (((uint64_t)Input[13]>>1) & 0b01111111);
			DTS = 0;
			//printf("TEST: 9=%d 10=%d 11=%d 12=%d 13=%d", Input[9], Input[10], Input[11], Input[12], Input[13]);
			//printf("TEST: 14=%d 15=%d 16=%d 17=%d 18=%d", Input[14], Input[15], Input[16], Input[17], Input[18]);
		}
		else{
			if (PTS_DTS_flag == 192) {
				totalHeaderLength += 10;
				if ((Input[9] & 0b11110000) != 0b00110000)
				{
					printf("PTS has wrong prefix!");
				}
				if ((Input[14] & 0b11110000) != 0b00010000)
				{
					printf("DTS has wrong prefix!");
				}
				PTS = ((((uint64_t)Input[9] >> 1) & 0b00000111) << 30) + ((uint64_t)Input[10] << 22) + ((((uint64_t)Input[11] >> 1) & 0b01111111) << 15) + (((uint64_t)Input[12] << 7)) + (((uint64_t)Input[13] >> 1) & 0b01111111);
				DTS = ((((uint64_t)Input[14] >> 1) & 0b00000111) << 30) + ((uint64_t)Input[15] << 22) + ((((uint64_t)Input[16] >> 1) & 0b01111111) << 15) + (((uint64_t)Input[17] << 7)) + (((uint64_t)Input[18] >> 1) & 0b01111111);
			}else {
				PTS = 0;
				DTS = 0;
			}
		}
		totalHeaderLength += m_PES_header_data_length;
	}
	
	return totalHeaderLength;
}

void xPES_PacketHeader::Print() const
{
	printf("PES: PSCP=%d SID=%d L=%d PTS=%d", m_PacketStartCodePrefix, m_StreamId, m_PacketLength, (long int)PTS);
}
