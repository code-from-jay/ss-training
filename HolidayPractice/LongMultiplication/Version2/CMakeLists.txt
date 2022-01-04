cmake_minimum_required(VERSION 3.14)
project(long_multiplication)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build tests" OFF)

if (BUILD_TESTS)
	include(FetchContent)
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY "https://github.com/google/googletest.git"
		GIT_TAG "release-1.11.0")
	FetchContent_MakeAvailable(googletest)

	enable_testing()

	add_executable(${PROJECT_NAME}_tests src/main_test.cpp src/BigNumber.cpp)
	target_link_libraries(${PROJECT_NAME}_tests gtest_main)

	include(GoogleTest)
	gtest_discover_tests(${PROJECT_NAME}_tests)
else()
	add_executable(${PROJECT_NAME} src/main.cpp src/BigNumber.cpp)
endif()
