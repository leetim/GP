cmake_minimum_required(VERSION 3.0)

project(tests)
add_definitions(-Wall -ansi -pthread -std=c++11 -Wall)
set(CMAKE_EXE_LINKER_FLAGS "-pthread")

#Подключение GoogleTest
add_library(gtest STATIC IMPORTED)
set_property(TARGET gtest PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/gtestLib/libgtest.a)
add_library(gtest_main STATIC IMPORTED)
set_property(TARGET gtest_main PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/gtestLib/libgtest_main.a)
add_library(gmock STATIC IMPORTED)
set_property(TARGET gmock PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/gtestLib/libgmock.a)
add_library(gmock_main STATIC IMPORTED)
set_property(TARGET gmock_main PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/gtestLib/libgmock_main.a)
include_directories(./gtestInclude)
set(GTEST_LINKS gtest gtest_main gmock gmock_main)

#Исходники тестов и их линковка
add_executable(test1 test.cpp)
target_link_libraries(test1 gtest gtest_main gmock gmock_main)

#Добавление тестов
add_test(NAME Alloc_Test COMMAND test1)