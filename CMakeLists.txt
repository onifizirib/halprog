cmake_minimum_required(VERSION 3.0.0)
project(gyak04 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(gyak04 main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
