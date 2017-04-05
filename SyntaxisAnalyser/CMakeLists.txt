cmake_minimum_required(VERSION 3.0)

project(syntaxisAnalyser)
add_definitions(-Wall -ansi -pthread -std=c++11 -Wall)
set(CMAKE_EXE_LINKER_FLAGS "-pthread")

#Заголовки
include_directories(./include)

#Бинарники
set( SA_SOURCE ./src/syntaxis_analyser.cpp ./src/syntax_tree.cpp ./src/symbol_table.cpp ./src/symbol.cpp )
add_library( syntaxisAnalyser STATIC ${SA_SOURCE} )
target_link_libraries( syntaxisAnalyser lexicalSearch)
