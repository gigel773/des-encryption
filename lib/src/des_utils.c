/* ------ Includes ------ */

#include <des_utils.h>
#include <des_tables.h>

/* ------ Defines ------ */

#define FIRST_28_BITS_MASK 0xFFFFFFF
#define SECOND_28_BITS_MASK 0xFFFFFFF0000000
#define NUMBER_OF_SIX_BIT_BLOCKS 8
#define ROW_MASK 0x21
#define COLUMN_MASK 0x30

/* ------ Internal service functions ------ */

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
        if (_bittest64((const long long *) &highPart, DES_EXTENDING_TABLE[i])) {
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
        if (_bittest64((const long long *) &compressedData, DES_FEISTEL_FUNCTION_FINAL_PERMUTATION_TABLE[i])) {
            _bittestandset64((long long *) &result, i);
        }
    }

    return result;
}

/* ------ Main functions implementation ------ */

void ownGenerateKeys(const char *key, char *generatedKeys) {

    /* Variables */
    long long    unitedKey   = 0;
    long long    temporalKey = 0;
    unsigned int keyLowPart  = 0;
    unsigned int keyHighPart = 0;
    unsigned int cyclicMask  = 0;

    /* Copying key bits */
    unitedKey |= *(unsigned int *) key;                               /* Copy first 32 bits */
    unitedKey |= ((long long) *(unsigned short *) (key + 4u) << 32u); /* Copy next 16 bits */
    unitedKey |= ((long long) *(key + 6u) << 48u);                    /* Copy last 8 bits */

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
        keyLowPart  = ((keyLowPart << DES_KEY_SHIFT_TABLE[i]) & FIRST_28_BITS_MASK) |
                      ((keyLowPart & cyclicMask) >> (28u - DES_KEY_SHIFT_TABLE[i]));
        keyHighPart = ((keyHighPart << DES_KEY_SHIFT_TABLE[i]) & FIRST_28_BITS_MASK) |
                      ((keyHighPart & cyclicMask) >> (28u - DES_KEY_SHIFT_TABLE[i]));

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
                   generatedKeys + i * DEFAULT_DES_ITERATION_KEY_BYTE_SIZE,
                   DEFAULT_DES_ITERATION_KEY_BYTE_SIZE);

    }
}

void ownReverseGeneratedKeys(char *generatedKeys) {

    /* Variables */
    char         reversedKeys[DEFAULT_DES_KEY_BYTE_SIZE * DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES];
    unsigned int counter = 0;

    /* Complete temporal buffer */
    for (unsigned int i = DEFAULT_DES_ITERATION_KEY_BYTE_SIZE * (DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES - 1);
         i > 0;
         i -= DEFAULT_DES_ITERATION_KEY_BYTE_SIZE) {
        COPY_ARRAY(generatedKeys + i, reversedKeys + counter, DEFAULT_DES_ITERATION_KEY_BYTE_SIZE)
        counter += DEFAULT_DES_ITERATION_KEY_BYTE_SIZE;
    }

    /* Copy reversed keys to the output */
    COPY_ARRAY(reversedKeys, generatedKeys, DEFAULT_DES_ITERATION_KEY_BYTE_SIZE * DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES);
}

void ownEncryptionProcessor(unsigned int *lowPart, unsigned int *highPart, char *keys) {

    /* Main actions */
    unsigned int temporalLowPart  = *highPart;
    unsigned int temporalHighPart = *lowPart ^feistelFunction(*highPart, keys);

    /* Switch to next iteration */
    *lowPart  = temporalLowPart;
    *highPart = temporalHighPart;
}

void ownDecryptionProcessor(unsigned int *lowPart, unsigned int *highPart, char *keys) {

    /* Main actions */
    unsigned int temporalHighPart = *lowPart;
    unsigned int temporalLowPart  = *highPart ^feistelFunction(*highPart, keys);

    /* Switch to next iteration */
    *lowPart  = temporalLowPart;
    *highPart = temporalHighPart;
}

void ownProcessBlock(long long block, char *generatedKeys, long long *output, uucInplaceFunction coreProcessor) {

    /* Variables */
    long long    buffer   = 0;
    unsigned int lowPart  = 0;
    unsigned int highPart = 0;

    /* Initial permutation */
    for (unsigned int i = 0; i < DEFAULT_DES_BLOCK_BIT_SIZE; i++) {
        if (_bittest64(&block, DES_INITIAL_PERMUTATION_TABLE[i])) {
            _bittestandset64(&buffer, i);
        }
    }

    /* Encrypting cycles */
    lowPart  = *(unsigned int *) (&buffer);
    highPart = *((unsigned int *) (&buffer) + 1);

    for (unsigned int i = 0; i < DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES; i++) {
        coreProcessor(&lowPart, &highPart, generatedKeys + i * DEFAULT_DES_ITERATION_KEY_BYTE_SIZE);
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

    /* Writing results */
    *output = buffer;
}
