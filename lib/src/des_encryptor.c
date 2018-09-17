/* ------ Includes ------ */

#include <des.h>

/* ------ Internal service functions ------ */

static void encryptBlock(long long block, char key[DEFAULT_DES_KEY_BYTE_SIZE]) {

    /* Variables */
    long long buffer = 0;

    /* Initial permutation */
    for (unsigned int i = 0; i < DEFAULT_DES_BLOCK_BIT_SIZE; i++) {
        _bittest64(&block, i) ? _bittestandset64(&buffer, DES_INITIAL_PERMUTATION_TABLE[i])
                              : _bittestandreset64(&buffer, DES_INITIAL_PERMUTATION_TABLE[i]);
    }

    /* Encrypting cycles */

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
