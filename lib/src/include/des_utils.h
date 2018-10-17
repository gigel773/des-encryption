#ifndef OWNDES_DES_UTILS_H
#define OWNDES_DES_UTILS_H

#include <definitions.h>

void ownGenerateKeys(const char *key, char *generatedKeys);

void ownProcessBlock(long long block, char *generatedKeys, long long *output);

#endif //OWNDES_DES_UTILS_H
