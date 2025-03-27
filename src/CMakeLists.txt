

# Add the library
add_library(csp SHARED csp.c csp.h csp.inc)

# Set the target include directory
target_include_directories(csp PUBLIC ${CMAKE_SOURCE_DIR})


