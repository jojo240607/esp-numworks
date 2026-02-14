#include <ion.h>

#include "svcall.h"
#include "esp_crc.h"


namespace Ion {

uint32_t SVC_ATTRIBUTES crc32DoubleWord(const uint32_t* data, size_t length){
    //    SVC_RETURNING_R0(SVC_CRC32_WORD, uint32_t)
    // ESP-IDF 提供的 CRC 函数
    uint32_t crc = esp_crc32_le(0xFFFFFFFF, (uint8_t const *)data, length * sizeof(uint32_t));
    return crc;
    //return 0;
}

uint32_t SVC_ATTRIBUTES crc32Byte(const uint8_t* data, size_t length) {
  //SVC_RETURNING_R0(SVC_CRC32_BYTE, uint32_t)
    uint32_t crc = esp_crc32_le(0xFFFFFFFF, (uint8_t const *)data, length);
    return crc;
}

}  // namespace Ion
