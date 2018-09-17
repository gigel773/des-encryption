#ifndef OWNDES_DEFINITIONS_H
#define OWNDES_DEFINITIONS_H

/* ------ Defines ------ */

#define DEFAULT_DES_KEY_BYTE_SIZE 7
#define DEFAULT_DES_BLOCK_BIT_SIZE 64
#define DEFAULT_DES_EXTENDING_TABLE_SIZE 48

/* ------ Macros ------ */

#define COPY_ARRAY(source, destination, numberOfElements) \
    for (unsigned int i = 0; i < (numberOfElements); i++) (destination)[i] = (source)[i];

#define FLUSH_ARRAY(array, size) \
    for (unsigned int i = 0; i < (size); i++) (array)[i] = 0;

/* ------ Enums ------ */

typedef enum {
    SUCCESS,
    FAILURE
} DES_status;

/* ------ Structures ------ */

struct DES_context {
    unsigned long long *source;
    unsigned long long *output;
    unsigned int sourceSize;
    char key[DEFAULT_DES_KEY_BYTE_SIZE];
};

#endif //OWNDES_DEFINITIONS_H
