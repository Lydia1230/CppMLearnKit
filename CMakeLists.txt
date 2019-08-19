cmake_minimum_required(VERSION 3.13)
project(cppMLKit)

set(CMAKE_CXX_STANDARD 14)

file(GLOB HEADERS "cppMLKit/*.h")
file(GLOB LIB_SRC "cppMLKitLib/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(cppMLKit ${LIB_SRC} ${HEADERS})

add_executable(myML main.cpp ${HEADERS})
target_link_libraries(myML cppMLKit)

add_executable(testMyVector example/testMyVector.cpp ${HEADERS})
target_link_libraries(testMyVector cppMLKit)

add_executable(testLogReg example/testLogReg.cpp ${HEADERS})
target_link_libraries(testLogReg cppMLKit)


add_executable(decisionTree example/testDecisionTree.cpp ${HEADERS})
target_link_libraries(decisionTree cppMLKit)