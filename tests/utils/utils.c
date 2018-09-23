#include <utils.h>

TEST_status TEST_compareStrings(char *source1, char *source2, unsigned int size) {

    /* Main comparing cycle */
    for (unsigned int i = 0; i < size; i++) {
        if (source1[i] != source2[i]) {
            return ERROR;
        }
    }

    return PASSED;
}

TEST_status TEST_copy(char *source, char *destination, unsigned int size) {

    /* Parameters validation */

    /* Main cycle */
    for (unsigned int i = 0; i < size; i++) {
        destination[i] = source[i];
    }

    return PASSED;
}
