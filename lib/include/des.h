#ifndef OWNDES_DES_H
#define OWNDES_DES_H

/* ------ Includes ------ */

#include <stdlib.h>
#include <definitions.h>

/* ------ Functions API ------ */

/**
 * Creates context for further encrypting or decrypting with DES algorithm
 *
 * @param context pointer to @code DES_context @endcode structure that has to be initialized
 * @param source pointer to data array that has to be encrypted
 * @param sourceSize size of data array
 * @param key 56-bit key for encryption
 * @return @code SUCCESS @endcode if everything was alright and @code FAILURE @endcode in the opposite case
 */
DES_status DES_createContext(struct DES_context *context,
                             unsigned long long *source,
                             unsigned int sourceSize,
                             char key[DEFAULT_DES_KEY_BYTE_SIZE]);

/**
 * Flushes initialized context (use after work)
 *
 * @param context pointer to @code DES_context @endcode structure that has to be flushed
 * @return @code SUCCESS @endcode if everything was alright and @code FAILURE @endcode in the opposite case
 */
DES_status DES_flushContext(struct DES_context *context);

DES_status DES_encrypt(struct DES_context *context);

DES_status DES_decrypt(struct DES_context *context);

#endif //OWNDES_DES_H
