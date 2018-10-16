/* ------ Includes ------ */

#include <des.h>

/* ------ Main functions implementation ------ */

DES_status DES_createContext(struct DES_context *context,
                             unsigned long long *source,
                             unsigned int sourceSize,
                             char key[DEFAULT_DES_KEY_BYTE_SIZE]) {

    /* Memory allocation */
    context->output = (unsigned long long *) malloc(sizeof(unsigned long long) * (sourceSize >> 3u));

    /* Simple assignment */
    context->source     = source;
    context->sourceSize = (sourceSize >> 3u);
    COPY_ARRAY(key, context->key, DEFAULT_DES_KEY_BYTE_SIZE)

    return SUCCESS;
}

DES_status DES_flushContext(struct DES_context *context) {

    /* Memory freeing */
    free(context->output);

    /* Simple assignment */
    context->output     = NULL;
    context->sourceSize = 0;
    FLUSH_ARRAY(context->key, DEFAULT_DES_KEY_BYTE_SIZE)

    return SUCCESS;
}
