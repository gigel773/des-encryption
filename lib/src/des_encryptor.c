/* ------ Includes ------ */

#include <des.h>
#include <des_utils.h>

/* ------ Main functions implementation ------ */

DES_status DES_encrypt(struct DES_context *context) {

    /* Variables */
    char generatedKeys[DEFAULT_DES_KEY_BYTE_SIZE * DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES];

    /* Keys generation */
    ownGenerateKeys(context->key, generatedKeys);

    /* Main cycle */
    for (unsigned int i = 0; i < context->sourceSize; i++) {
        ownProcessBlock(context->source[i], context->key, (long long *) (context->output + i));
    }

    return SUCCESS;
}
