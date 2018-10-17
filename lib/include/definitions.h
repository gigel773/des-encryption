#ifndef OWNDES_DEFINITIONS_H
#define OWNDES_DEFINITIONS_H

/* ------ Includes ------ */
#ifdef _MSC_VER

#include <intrin.h>

#else

#include <immintrin.h>
#include <psdk_inc/intrin-impl.h>

#endif

/* ------ Defines ------ */

#define DEFAULT_DES_KEY_BYTE_SIZE 7
#define DEFAULT_DES_KEY_BIT_SIZE 56
#define DEFAULT_DES_BLOCK_BIT_SIZE 64
#define DEFAULT_DES_EXTENDING_TABLE_SIZE 48
#define DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES 16
#define DEFAULT_DES_S_BOXES_NUMBER 8

/* ------ Macros ------ */

#define COPY_ARRAY(source, destination, numberOfElements) \
    for (unsigned int index = 0; index < (numberOfElements); index++) (destination)[index] = (source)[index];

#define FLUSH_ARRAY(array, size) \
    for (unsigned int index = 0; index < (size); index++) (array)[index] = 0;

/* ------ Enums ------ */

typedef enum {
    SUCCESS,
    FAILURE
} DES_status;

typedef enum {
    ZERO   = 0,
    FIRST  = 1,
    SECOND = 32,
    THIRD  = 33
} SBOX_rows;

/* ------ Structures ------ */

struct DES_context {
    unsigned long long *source;
    unsigned long long *output;
    unsigned int       sourceSize;
    char               key[DEFAULT_DES_KEY_BYTE_SIZE];
};

#endif //OWNDES_DEFINITIONS_H
