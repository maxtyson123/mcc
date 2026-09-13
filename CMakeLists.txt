# Setup Cmake
CMAKE_MINIMUM_REQUIRED(VERSION 3.25)
SET(CMAKE_CXX_STANDARD 20)
CMAKE_POLICY(SET CMP0177 NEW)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup project
PROJECT(mcc C CXX)
FILE(GLOB_RECURSE COMPILER_SRCS CONFIGURE_DEPENDS src/*.cpp src/*.s)
SET(TEST_COMPILER_SRCS ${COMPILER_SRCS})
LIST(FILTER TEST_COMPILER_SRCS EXCLUDE REGEX "main\\.cpp$")
FILE(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS tests/*.cpp)

# Create compiler
ADD_EXECUTABLE(mcc ${COMPILER_SRCS})
TARGET_INCLUDE_DIRECTORIES(mcc PRIVATE include)

# Get the catch 2 framework
INCLUDE(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)

# Create tests, and remove entry point to allow for test framework to inject own
ADD_EXECUTABLE(mcc_tests ${TEST_SRCS} ${TEST_COMPILER_SRCS})
TARGET_INCLUDE_DIRECTORIES(mcc_tests PRIVATE include)
TARGET_LINK_LIBRARIES(mcc_tests PRIVATE Catch2::Catch2WithMain)

# Enable testing
ENABLE_TESTING()
INCLUDE(CTest)
INCLUDE(Catch)
catch_discover_tests(mcc_tests)
