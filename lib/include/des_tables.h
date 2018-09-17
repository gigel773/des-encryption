#ifndef OWNDES_DES_TABLES_H
#define OWNDES_DES_TABLES_H

#include <definitions.h>

unsigned int DES_INITIAL_PERMUTATION_TABLE[DEFAULT_DES_BLOCK_BIT_SIZE] = {
        0x3A, 0x32, 0x2A, 0x22, 0x1A, 0x12, 0x0A, 0x02, 0x3C, 0x34, 0x2C, 0x24, 0x1C, 0x14, 0x0C, 0x04,
        0x3E, 0x36, 0x2E, 0x26, 0x1E, 0x16, 0x0E, 0x06, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08,
        0x39, 0x31, 0x29, 0x21, 0x19, 0x11, 0x09, 0x01, 0x3B, 0x33, 0x2B, 0x23, 0x1B, 0x13, 0x0B, 0x03,
        0x3D, 0x35, 0x2D, 0x25, 0x1D, 0x15, 0x0D, 0x05, 0x3F, 0x37, 0x2F, 0x27, 0x1F, 0x17, 0x0F, 0x07,
};

unsigned int DES_EXTENDING_TABLE[DEFAULT_DES_EXTENDING_TABLE_SIZE] = {
        0x20, 0x01, 0x02, 0x03, 0x04, 0x05,
        0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
        0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
        0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
        0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x01,
};

#endif //OWNDES_DES_TABLES_H
