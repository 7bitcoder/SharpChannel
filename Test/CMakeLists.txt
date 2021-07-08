

set(CopyFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/socketServerTester.py
    ${CMAKE_CURRENT_SOURCE_DIR}/socketClientTester.py
)

file(COPY ${CopyFiles}
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

file(COPY ${CopyFiles}
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(Test 
    RunTests.cpp
    #RunCommandTest.cpp
    #ChannelBehaviourTest.cpp
    #SocketServerTest.cpp
    #SocketClientTest.cpp
)

target_link_libraries(Test 
    SharpChannel
    CONAN_PKG::gtest
)

target_include_directories(Test PRIVATE
    "../Source/.h"
)

gtest_discover_tests(Test Tests PROPERTIES TIMEOUT 10)