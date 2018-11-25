#ifndef OWNDES_DES_UTILS_H
#define OWNDES_DES_UTILS_H

#include <definitions.h>

void ownGenerateKeys(const char *key, char *generatedKeys);

void ownReverseGeneratedKeys(char *generatedKeys);

void ownEncryptionProcessor(unsigned int *lowPart, unsigned int *highPart, char *keys);

void ownDecryptionProcessor(unsigned int *lowPart, unsigned int *highPart, char *keys);

void ownProcessBlock(long long block, char *generatedKeys, long long *output, uucInplaceFunction coreProcessor);

#endif //OWNDES_DES_UTILS_H
