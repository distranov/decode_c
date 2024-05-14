#include "crc32.h"

// CRC-32D --------------------------------------------------------------------
uint32_t CRC32_Soft(uint32_t *pSrc, uint32_t len) {
    uint32_t crc = 0xFFFFFFFF;
    for (int k = 0; k < len; k++) {
        crc = crc ^ pSrc[k];
        for(uint8_t i = 0; i < 32; i++) {
            if(crc & 0x80000000)
                crc = (crc << 1) ^ 0xA833982B;
            else 
                crc = (crc << 1);
        }
    }
    return(crc);
}