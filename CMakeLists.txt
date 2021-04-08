cmake_minimum_required(VERSION 3.1.2)
project(pipe VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Cmake")
include(Config)

include(GoogleTest)
enable_testing()

include_directories(
    "Inlcude/SharpChannel"
)

add_subdirectory(Source)
add_subdirectory(Test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
