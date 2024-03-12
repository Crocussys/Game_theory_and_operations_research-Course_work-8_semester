cmake_minimum_required(VERSION 3.5)

project(Game_theory_and_operations_research-Course_work-8_semester LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Game_theory_and_operations_research-Course_work-8_semester main.cpp
    mat.h mat.cpp
    maxmin.h
    tests/test1
    tests/test2
    tests/test3 )

include(GNUInstallDirs)
install(TARGETS Game_theory_and_operations_research-Course_work-8_semester
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
