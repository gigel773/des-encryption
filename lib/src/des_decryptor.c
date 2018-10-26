/* ------ Includes ------ */

#include <des.h>
#include <des_utils.h>

/* ------ Main functions implementation ------ */

DES_status DES_decrypt(struct DES_context *context) {

    /* Variables */
    char generatedKeys[DEFAULT_DES_ITERATION_KEY_BYTE_SIZE * DEFAULT_DES_FEISTEL_NUMBER_OF_CYCLES];

    /* Keys generation */
    ownGenerateKeys(context->key, generatedKeys);
    ownReverseGeneratedKeys(generatedKeys);

    /* Main cycle */
    for (unsigned int i = 0; i < context->sourceSize; i++) {
        ownProcessBlock(context->source[i], generatedKeys, (long long *) (context->output + i), ownDecryptionProcessor);
    }

    return SUCCESS;
}
