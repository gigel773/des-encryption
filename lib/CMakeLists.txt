project(ownDesLibrary C)

include_directories(include)
set(SOURCES src/des_context.c src/des_encryptor.c)
add_library(ownDes ${SOURCES})
