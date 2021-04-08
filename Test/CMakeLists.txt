

if(OS_WINDOWS)

set(PLATFORM_TEST_SRC
    Windows/Test.cpp
)

elseif(OS_LINUX OR OS_FREEBSD OR OS_OPENBSD OR OS_NETBSD)

set(PLATFORM_TEST_SRC
    Linux/Test.cpp
)

endif()


add_executable(Test
    RunTests.cpp
    ${PLATFORM_TEST_SRC}
)

target_link_libraries(Test
    SharpChannel
    CONAN_PKG::gtest
)

add_test(test-1 Test)
