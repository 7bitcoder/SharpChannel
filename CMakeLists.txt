cmake_minimum_required(VERSION 3.0.0)
project(pipe VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

include_directories(
    "Inlcude/SharpChannel"
)

add_subdirectory(Source)
add_subdirectory(Test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
