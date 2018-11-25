/* ------ Includes ------ */

#include <des.h>

/* ------ Main functions implementation ------ */

DES_status DES_createContext(struct DES_context *context,
                             unsigned long long *source,
                             unsigned int sourceSize,
                             char key[DEFAULT_DES_KEY_BYTE_SIZE]) {

    /* Memory allocation */
    context->source = (unsigned long long *) malloc(sizeof(unsigned long long) * (sourceSize >> 3u));
    context->output = (unsigned long long *) malloc(sizeof(unsigned long long) * (sourceSize >> 3u));

    /* Simple assignment */
    context->sourceSize = (sourceSize >> 3u);
    COPY_ARRAY(key, context->key, DEFAULT_DES_KEY_BYTE_SIZE)
    COPY_ARRAY(source, context->source, context->sourceSize)

    return SUCCESS;
}

DES_status DES_flushContext(struct DES_context *context) {

    /* Memory freeing */
    free(context->source);
    free(context->output);

    /* Simple assignment */
    context->source     = NULL;
    context->output     = NULL;
    context->sourceSize = 0;
    FLUSH_ARRAY(context->key, DEFAULT_DES_KEY_BYTE_SIZE)

    return SUCCESS;
}

DES_status DES_inputToOutput(struct DES_context *context) {

    COPY_ARRAY(context->output, context->source, context->sourceSize)
    FLUSH_ARRAY(context->output, context->sourceSize)

    return SUCCESS;
}
