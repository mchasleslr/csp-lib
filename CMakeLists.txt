cmake_minimum_required(VERSION 3.5)

# Set the project name
project(CSP)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Support for test names
if(POLICY CMP0110)
  cmake_policy(SET CMP0110 NEW)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.c")
message(STATUS "SOURCES=${SOURCES}")

file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/*.h")
message(STATUS "HEADERS=${HEADERS}")

add_library(csp SHARED ${SOURCES})
target_include_directories(csp PUBLIC ${CMAKE_SOURCE_DIR})
# set_target_properties(csp PROPERTIES VERSION ${PROJECT_VERSION})

# Add the executable
add_executable(solve-queens solve-queens.c)

# Link the library to the executable
target_link_libraries(solve-queens csp)

enable_testing()

add_subdirectory(tests)

# Add CPack
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "CSP")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
include(CPack)
