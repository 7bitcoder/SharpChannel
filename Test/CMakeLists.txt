
set(PythonDir ${CMAKE_CURRENT_SOURCE_DIR}/python)

set(CopyFiles 
    ${PythonDir}/socketServerTester.py
    ${PythonDir}/socketClientTester.py
    ${PythonDir}/echo.py
)

file(COPY ${CopyFiles}
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

file(COPY ${CopyFiles}
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(Test 
    RunTests.cpp
    CommandChannelTest.cpp
    ChannelBehaviourTest.cpp
    CommandOutputChannelTest.cpp
    #SocketServerTest.cpp
    #SocketClientChannelTest.cpp
)

target_link_libraries(Test 
    SharpChannel
    CONAN_PKG::gtest
)

target_include_directories(Test PRIVATE
    "../Source/.h"
)

gtest_discover_tests(Test Tests PROPERTIES TIMEOUT 10)