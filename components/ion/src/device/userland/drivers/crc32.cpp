#include <ion.h>

#include "svcall.h"

namespace Ion {

uint32_t SVC_ATTRIBUTES crc32DoubleWord(const uint32_t* data, size_t length){
    //    SVC_RETURNING_R0(SVC_CRC32_WORD, uint32_t)
    return 0;
}

uint32_t SVC_ATTRIBUTES crc32Byte(const uint8_t* data, size_t length) {
  //SVC_RETURNING_R0(SVC_CRC32_BYTE, uint32_t)
    return 0;
}

}  // namespace Ion
