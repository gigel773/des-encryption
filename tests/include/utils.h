#ifndef OWNDES_UTILS_H
#define OWNDES_UTILS_H

/* ------ Includes ------ */

#include <stdlib.h>

/* ------ Defines ------ */

#define TEST_MESSAGE_BYTE_SIZE 16

/* ------ Types ------ */

typedef enum {
    PASSED,
    ERROR
} TEST_status;

/* ------ Main test system functions API ------ */

TEST_status TEST_compareStrings(char *source1, char *source2, unsigned int size);

TEST_status TEST_copy(char *source, char *destination, unsigned int size);

#endif //OWNDES_UTILS_H
