#include "tsCommon.h"
#include "tsTransportStream.h"
#include "xTS_AdaptationField.h"
#include <cstdio>

int main( int argc, char *argv[ ], char *envp[ ])
{
  // TODO - open file
  FILE *stream= fopen("example_new.ts", "rb");
  if (stream == NULL) { printf("wrong file name\n"); return EXIT_FAILURE; }

  xTS_PacketHeader    TS_PacketHeader;
  xTS_AdaptationField TS_AdaptationField;

  uint8_t* TS_PacketBuffer;
  TS_PacketBuffer = (uint8_t*)malloc(sizeof(uint8_t) * xTS::TS_PacketLength);

  int32_t TS_PacketId = 0;
  while(!feof(stream)) {
    // TODO - read from file


    if (TS_PacketBuffer == NULL) { printf("Memory error"); TS_PacketId++; continue; }
    size_t readingLength = fread(TS_PacketBuffer, sizeof(uint8_t), xTS::TS_PacketLength, stream);

    if (readingLength != xTS::TS_PacketLength){
        printf("Packet reading failed.\n");
        TS_PacketId++;
        continue;
    }


    TS_PacketHeader.Reset();
    TS_PacketHeader.Parse(TS_PacketBuffer);

    uint8_t headerAFC = TS_PacketHeader.getAFC();

    TS_AdaptationField.Reset();

    TS_AdaptationField.Parse(TS_PacketBuffer, headerAFC);


    printf("%010d ", TS_PacketId);
    TS_PacketHeader.Print();
    printf(" ");
    if (headerAFC == 2 || headerAFC == 3) TS_AdaptationField.Print();
    printf("\n");

    if (TS_PacketId == 33) {
        int test = 1;
    }

    TS_PacketId++;
  }
  return 0;
}