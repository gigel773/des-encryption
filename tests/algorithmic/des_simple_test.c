#include <des.h>
#include <utils.h>

/**
 * Purpose of this test is to check if main DES functions are working correctly
 * by ciphering and deciphering messages of different length and then comparing
 * results after this actions
 *
 * This case is testing if the whole pipeline is working correctly:
 *  context creating -> ciphering -> deciphering -> context flushing
 *
 * @return 0 if test was successful and 1 in the opposite case
 */
int main() {

    /* Variables */
    char message[TEST_MESSAGE_BYTE_SIZE] = "aaaaaaaabbbbbbbb";
    char decipheredMessage[TEST_MESSAGE_BYTE_SIZE];
    char cipheredMessage[TEST_MESSAGE_BYTE_SIZE];
    char key[DEFAULT_DES_KEY_BYTE_SIZE]  = "1234567";

    struct DES_context context;

    /* Main test actions */
    DES_createContext(&context, (unsigned long long int *) message, TEST_MESSAGE_BYTE_SIZE, key);
    DES_encrypt(&context);
    TEST_copy((char *) context.output, cipheredMessage, TEST_MESSAGE_BYTE_SIZE);
    DES_decrypt(&context);
    TEST_copy((char *) context.output, decipheredMessage, TEST_MESSAGE_BYTE_SIZE);
    DES_flushContext(&context);

    /* Comparing results */
    return TEST_compareStrings(message, decipheredMessage, TEST_MESSAGE_BYTE_SIZE);
}
