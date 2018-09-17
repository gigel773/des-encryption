/* ------ Includes ------ */

#include <des.h>

/* ------ Defines ------ */

#define DES_ITERATION_KEY_BYTE_SIZE 6

/* ------ Internal service functions ------ */

static void generateKeys(const char key[DEFAULT_DES_KEY_BYTE_SIZE], char *generatedKeys) {

    /* Variables */
    long long unitedKey = 0;
    long long temporalKey = 0;

    /* Copying key bits */
    unitedKey |= *(unsigned int *) key;                             /* Copy first 32 bits */
    unitedKey |= ((long long) *(unsigned short *) (key + 4) << 31); /* Copy next 16 bits */
    unitedKey |= ((long long) *(key + 6) << 47);                    /* Copy last 8 bits */

    /* Initial permutation */
    for (unsigned int i = 0; i < DEFAULT_DES_KEY_BIT_SIZE; i++) {

        _bittest64(&unitedKey, i) ? _bittestandset64(&temporalKey, DES_KEY_INITIAL_PERMUTATION_TABLE[i])
                                  : _bittestandreset64(&temporalKey, DES_KEY_INITIAL_PERMUTATION_TABLE[i]);
    }

    unitedKey = temporalKey;
    temporalKey = 0;
}

static unsigned int mainEncryptionFunction(unsigned int highPart, char key[DES_ITERATION_KEY_BYTE_SIZE]) {

    /* Variables */

    return 0;
}

static void encryptBlock(long long block, char key[DEFAULT_DES_KEY_BYTE_SIZE]) {

    /* Variables */
    long long buffer = 0;
    unsigned int lowPart = 0;
    unsigned int highPart = 0;
    unsigned int temporalLowPart = 0;
    unsigned int temporalHighPart = 0;
    char *generatedKeys = NULL;

    /* Initial permutation */
    for (unsigned int i = 0; i < DEFAULT_DES_BLOCK_BIT_SIZE; i++) {

        _bittest64(&block, i) ? _bittestandset64(&buffer, DES_INITIAL_PERMUTATION_TABLE[i])
                              : _bittestandreset64(&buffer, DES_INITIAL_PERMUTATION_TABLE[i]);
    }

    /* Keys generation */
    generateKeys(key, generatedKeys);

    /* Encrypting cycles */
    lowPart = *(unsigned int *) (&buffer);
    highPart = *((unsigned int *) (&buffer) + 1);

    for (unsigned int i = 0; i < DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES; i++) {

        /* Main actions */
        temporalLowPart = highPart;
        temporalHighPart = lowPart ^ mainEncryptionFunction(highPart,
                                                            generatedKeys + (i * DES_ITERATION_KEY_BYTE_SIZE));

        /* Switch to next iteration */
        lowPart = temporalLowPart;
        highPart = temporalHighPart;
    }

    /* Final permutation */

}

/* ------ Main functions implementation ------ */

DES_status DES_encrypt(struct DES_context *context) {

    /* Variables */

    /* Main cycle */
    for (unsigned int i = 0; i < context->sourceSize; i++) {
        encryptBlock(context->source[i], context->key);
    }

    return SUCCESS;
}
