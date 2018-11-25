project(ownDesLibrary C)

include_directories(include src/include)
set(SOURCES src/des_context.c src/des_encryptor.c src/des_decryptor.c src/include/des_utils.h src/des_utils.c)
add_library(ownDes ${SOURCES})
