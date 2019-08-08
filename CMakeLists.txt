cmake_minimum_required(VERSION 3.13)
project(myML)

set(CMAKE_CXX_STANDARD 14)

file(GLOB HEADERS "cppMLKit/*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(myML main.cpp ${HEADERS})
add_executable(testMyVector tests/testMyVector.cpp ${HEADERS})
add_executable(testLogReg tests/testLogReg.cpp ${HEADERS})
