

if(OS_WINDOWS)

set(PLATFORM_TEST_SRC
    Windows/RunCommandTest.cpp
    Windows/ChannelBehaviourTest.cpp
    Windows/SocketServerTest.cpp
)

elseif(OS_LINUX OR OS_FREEBSD OR OS_OPENBSD OR OS_NETBSD)

set(PLATFORM_TEST_SRC
    Linux/RunCommandTest.cpp
)

endif()


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/socketServerTester.py
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

add_executable(Test 
    RunTests.cpp
    ${PLATFORM_TEST_SRC}
)

target_link_libraries(Test 
    SharpChannel
    CONAN_PKG::gtest
)

add_test(test-1 bin/Test)