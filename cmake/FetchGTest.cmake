include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(GTEST_BUILD_GMOCK OFF CACHE BOOL "Disable building GoogleMock")
set(GTEST_BUILD_TESTS OFF CACHE BOOL "Disable building GoogleTest tests")

FetchContent_MakeAvailable(googletest)