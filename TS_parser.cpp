#include "tsCommon.h"
#include "tsTransportStream.h"
#include "xTS_AdaptationField.h"
#include <cstdio>
#include "TS_parser.h"
#include "xPES_Assembler.h"
#include <vector>


int main( int argc, char *argv[ ], char *envp[ ])
{

  FILE *stream= fopen("example_new.ts", "rb");
  if (stream == NULL) { printf("wrong file name\n"); return EXIT_FAILURE; }

  xTS_PacketHeader    TS_PacketHeader;
  xTS_AdaptationField TS_AdaptationField;
  xPES_Assembler PES_Assembler;

  PES_Assembler.Init(136);

  xTS_PacketHeader    TS_PacketHeader2;
  xTS_AdaptationField TS_AdaptationField2;
  xPES_Assembler PES_Assembler2;

  PES_Assembler2.Init(174);

  uint8_t* TS_PacketBuffer;
  TS_PacketBuffer = (uint8_t*)malloc(sizeof(uint8_t) * xTS::TS_PacketLength);
  if (TS_PacketBuffer == NULL) { printf("Memory error");  return 0; }

  int32_t TS_PacketId = 0;
  while(!feof(stream)) {

    size_t readingLength = fread(TS_PacketBuffer, sizeof(uint8_t), xTS::TS_PacketLength, stream);

    if (readingLength != xTS::TS_PacketLength){
        printf("Packet reading failed.\n");
        TS_PacketId++;
        continue;
    }


    TS_PacketHeader.Reset();
    TS_PacketHeader.Parse(TS_PacketBuffer);

    TS_AdaptationField.Reset();


    TS_PacketHeader2.Reset();
    TS_PacketHeader2.Parse(TS_PacketBuffer);

    TS_AdaptationField2.Reset();

    if (TS_PacketHeader.getSyncByte() == 'G' && TS_PacketHeader.getPID() == 136) {

        if (TS_PacketHeader.hasAdaptationField()) TS_AdaptationField.Parse(TS_PacketBuffer, TS_PacketHeader.getAdaptationFieldControl());

        printf("%010d ", TS_PacketId);
        TS_PacketHeader.Print();
        printf(" ");
        if (TS_PacketHeader.hasAdaptationField()) TS_AdaptationField.Print();

        xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader, &TS_AdaptationField);
        switch (Result)
        {
        case xPES_Assembler::eResult::StreamPackedLost: printf("PcktLost "); break;
        case xPES_Assembler::eResult::AssemblingStarted: printf("Started "); PES_Assembler.PrintPESH(); break;
        case xPES_Assembler::eResult::AssemblingContinue: printf("Continue "); break;
        case xPES_Assembler::eResult::AssemblingFinished: printf("Finished "); printf("PES: PcktLen=%d HeadLen=%d DataLen=%d", PES_Assembler.getNumPacketBytes(), PES_Assembler.getHeaderSize(), PES_Assembler.getNumPacketBytes() - PES_Assembler.getHeaderSize()); break; // saving to file is in PES assembler
        default: break;
        }
        printf("\n");
    }

    if (TS_PacketHeader2.getSyncByte() == 'G' && TS_PacketHeader2.getPID() == 174) {

        if (TS_PacketHeader2.hasAdaptationField()) TS_AdaptationField2.Parse(TS_PacketBuffer, TS_PacketHeader2.getAdaptationFieldControl());

        printf("%010d ", TS_PacketId);
        TS_PacketHeader2.Print();
        printf(" ");
        if (TS_PacketHeader2.hasAdaptationField()) TS_AdaptationField2.Print();

        xPES_Assembler::eResult Result = PES_Assembler2.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader2, &TS_AdaptationField2);
        switch (Result)
        {
        case xPES_Assembler::eResult::StreamPackedLost: printf("PcktLost "); break;
        case xPES_Assembler::eResult::AssemblingStarted: printf("Started "); PES_Assembler2.PrintPESH(); break;
        case xPES_Assembler::eResult::AssemblingContinue: printf("Continue "); break;
        case xPES_Assembler::eResult::AssemblingFinished: printf("Finished "); printf("PES: PcktLen=%d HeadLen=%d DataLen=%d", PES_Assembler2.getNumPacketBytes(), PES_Assembler2.getHeaderSize(), PES_Assembler2.getNumPacketBytes() - PES_Assembler2.getHeaderSize()); break; // saving to file is in PES assembler
        default: break;
        }
        printf("\n");
    }

    TS_PacketId++;
  }
  return 0;
}