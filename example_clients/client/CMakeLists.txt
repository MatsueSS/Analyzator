cmake_minimum_required(VERSION 3.10)
project(Client C)

set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(OpenSSL REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.c
    src/needed.c
    src/cipher.c
    src/decipher.c
)

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL OpenSSL::Crypto)
