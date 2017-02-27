cmake_minimum_required(VERSION 3.0)

project(LexicalSearch)
add_definitions(-Wall -ansi -pthread -std=c++11 -Wall)
set(CMAKE_EXE_LINKER_FLAGS "-pthread")

#Заголовки
include_directories(./include)

#Бинарники
set( LEXICAL_SEARCH_SOURCE ./src/lexicalSearch.cpp ./src/machine.cpp ./src/defs.cpp )
add_library( lexicalSearch STATIC ${LEXICAL_SEARCH_SOURCE} )
