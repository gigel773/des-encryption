project(ownDesLibrary C)

include_directories(include)
set(SOURCES src/des_context.c src/des_encryptor.c src/des_decryptor.c)
add_library(ownDes ${SOURCES})
