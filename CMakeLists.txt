cmake_minimum_required(VERSION 3.0)

project(GP_main)
add_definitions(-Wall -ansi -pthread -std=c++11 -Wall)
set(CMAKE_EXE_LINKER_FLAGS "-pthread")

#Включение тестов
enable_testing()

#Заголовочные файлы
include_directories(./LexicalSearch/include)
include_directories(./SyntaxisAnalyser/include)

#Подпроекты
add_subdirectory(LexicalSearch ./LexicalSearch)
add_subdirectory(SyntaxisAnalyser ./SyntaxisAnalyser)
add_subdirectory(tests ./tests)


set(GP_MAIN_SOURCE ./main.cpp)
add_executable(gp ${GP_MAIN_SOURCE})
target_link_libraries(gp lexicalSearch syntaxisAnalyser)
