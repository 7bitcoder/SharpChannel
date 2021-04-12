

set(CopyFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/socketServerTester.py
)

file(COPY ${CopyFiles}
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

file(COPY ${CopyFiles}
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(Test 
    RunTests.cpp
    RunCommandTest.cpp
    ChannelBehaviourTest.cpp
    SocketServerTest.cpp
)

target_link_libraries(Test 
    SharpChannel
    CONAN_PKG::gtest
)

gtest_discover_tests(Test Tests PROPERTIES TIMEOUT 10)