/* ------ Includes ------ */

#include <des.h>
#include <des_tables.h>

/* ------ Defines ------ */

#define DES_ITERATION_KEY_BYTE_SIZE 6
#define FIRST_28_BITS_MASK 0xFFFFFFF
#define SECOND_28_BITS_MASK 0xFFFFFFF0000000
#define NUMBER_OF_SIX_BIT_BLOCKS 8
#define ROW_MASK 0x21
#define COLUMN_MASK 0x30

/* ------ Internal service functions ------ */

static void generateKeys(const char key[DEFAULT_DES_KEY_BYTE_SIZE], char *generatedKeys) {

    /* Variables */
    long long    unitedKey   = 0;
    long long    temporalKey = 0;
    unsigned int keyLowPart  = 0;
    unsigned int keyHighPart = 0;
    unsigned int cyclicMask  = 0;

    /* Copying key bits */
    unitedKey |= *(unsigned int *) key;                               /* Copy first 32 bits */
    unitedKey |= ((long long) *(unsigned short *) (key + 4u) << 31u); /* Copy next 16 bits */
    unitedKey |= ((long long) *(key + 6u) << 47u);                    /* Copy last 8 bits */

    /* Initial permutation */
    for (unsigned int i = 0; i < DEFAULT_DES_KEY_BIT_SIZE; i++) {
        if (_bittest64(&unitedKey, DES_KEY_INITIAL_PERMUTATION_TABLE[i])) {
            _bittestandset64(&temporalKey, i);
        }
    }

    keyLowPart  = (unsigned int) (temporalKey & FIRST_28_BITS_MASK);
    keyHighPart = (unsigned int) ((temporalKey & SECOND_28_BITS_MASK) >> 28u);

    /* Main generation cycle */
    for (unsigned int i = 0; i < DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES; i++) {

        /* Cyclic shift */
        cyclicMask  = ((1u << DES_KEY_SHIFT_TABLE[i]) - 1u) << (28u - DES_KEY_SHIFT_TABLE[i]);
        keyLowPart  = ((keyLowPart << DES_KEY_SHIFT_TABLE[i]) & FIRST_28_BITS_MASK) | (keyLowPart & cyclicMask);
        keyHighPart = ((keyHighPart << DES_KEY_SHIFT_TABLE[i]) & FIRST_28_BITS_MASK) | (keyHighPart & cyclicMask);

        /* Extending permutation */
        unitedKey   = (((long long) keyHighPart) << 28u) | (keyLowPart);
        temporalKey = 0;

        for (unsigned int j = 0; j < DEFAULT_DES_EXTENDING_TABLE_SIZE; j++) {
            if (_bittest64(&unitedKey, DES_KEY_EXTENDING_PERMUTATION_TABLE[j])) {
                _bittestandset64(&temporalKey, j);
            }
        }

        /* Writing results */
        COPY_ARRAY((char *) &temporalKey,
                   generatedKeys + index * DES_ITERATION_KEY_BYTE_SIZE,
                   DES_ITERATION_KEY_BYTE_SIZE);

    }
}

static unsigned int feistelFunction(unsigned int highPart, const char *key) {

    /* Variables */
    long long int extendedData   = 0;
    unsigned char rowId          = 0;
    unsigned char columnId       = 0;
    unsigned char fourBitValue   = 0;
    unsigned int  compressedData = 0;
    unsigned int  result         = 0;

    /* Extending */
    for (unsigned int i = 0; i < DEFAULT_DES_EXTENDING_TABLE_SIZE; i++) {
        if (_bittest((const long *) &highPart, (long) DES_EXTENDING_TABLE[i])) {
            _bittestandset64(&extendedData, i);
        }
    }

    /* XOR with the key */
    extendedData = (extendedData ^ *(long long int *) key) & ((1ll << 48) - 1);

    /* S-blocks transform */
    for (unsigned int i = 0; i < NUMBER_OF_SIX_BIT_BLOCKS; i++) {

        /* Calculating position of new 4-bit value */
        rowId    = (unsigned char) ((extendedData >> (i * 6)) & ROW_MASK);
        columnId = (unsigned char) ((extendedData >> (i * 6)) & COLUMN_MASK);

        /* Getting new 4-bit value from  */
        switch (rowId) {
            case ZERO:
                fourBitValue = (unsigned char) DES_S_BOX_TRANSFORM_TABLE[i][0x00 + columnId];
                break;
            case FIRST:
                fourBitValue = (unsigned char) DES_S_BOX_TRANSFORM_TABLE[i][0x10 + columnId];
                break;
            case SECOND:
                fourBitValue = (unsigned char) DES_S_BOX_TRANSFORM_TABLE[i][0x20 + columnId];
                break;
            case THIRD:
                fourBitValue = (unsigned char) DES_S_BOX_TRANSFORM_TABLE[i][0x30 + columnId];
                break;
            default:
                break;
        }

        /* Writing 4-bit value to output */
        compressedData |= (fourBitValue << (i * 4));
    }

    /* Permutation */
    for (unsigned int i = 0; i < DEFAULT_DES_EXTENDING_TABLE_SIZE; i++) {
        if (_bittest((const long *) &compressedData, (long) DES_FEISTEL_FUNCTION_FINAL_PERMUTATION_TABLE[i])) {
            _bittestandset((long *) &result, (long) i);
        }
    }

    return result;
}

static void encryptBlock(long long block, char key[DEFAULT_DES_KEY_BYTE_SIZE], long long *output) {

    /* Variables */
    long long    buffer           = 0;
    unsigned int lowPart          = 0;
    unsigned int highPart         = 0;
    unsigned int temporalLowPart  = 0;
    unsigned int temporalHighPart = 0;
    char         generatedKeys[DEFAULT_DES_KEY_BYTE_SIZE * DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES];

    /* Initial permutation */
    for (unsigned int i = 0; i < DEFAULT_DES_BLOCK_BIT_SIZE; i++) {
        if (_bittest64(&block, DES_INITIAL_PERMUTATION_TABLE[i])) {
            _bittestandset64(&buffer, i);
        }
    }

    /* Keys generation */
    generateKeys(key, generatedKeys);

    /* Encrypting cycles */
    lowPart  = *(unsigned int *) (&buffer);
    highPart = *((unsigned int *) (&buffer) + 1);

    for (unsigned int i = 0; i < DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES; i++) {

        /* Main actions */
        temporalLowPart  = highPart;
        temporalHighPart = lowPart ^ feistelFunction(highPart,
                                                     generatedKeys + i * DES_ITERATION_KEY_BYTE_SIZE);

        /* Switch to next iteration */
        lowPart  = temporalLowPart;
        highPart = temporalHighPart;
    }

    /* Final permutation */
    buffer = 0;
    buffer |= lowPart;
    buffer |= highPart << 31u;

    for (unsigned int i = 0; i < DEFAULT_DES_BLOCK_BIT_SIZE; i++) {
        if (_bittest64(&buffer, DES_FINAL_PERMUTATION_TABLE[i])) {
            _bittestandset64(output, i);
        }
    }
}

/* ------ Main functions implementation ------ */

DES_status DES_encrypt(struct DES_context *context) {

    /* Variables */

    /* Main cycle */
    for (unsigned int i = 0; i < context->sourceSize; i++) {
        encryptBlock(context->source[i], context->key, (long long *) (context->output + i));
    }

    return SUCCESS;
}
