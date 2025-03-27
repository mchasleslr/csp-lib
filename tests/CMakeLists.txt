# Find test files
file(GLOB FILES "${CMAKE_CURRENT_SOURCE_DIR}/test-*.c")

find_program(VALGRIND valgrind)

# Add tests
foreach(FILENAME ${FILES})
  get_filename_component(SRC ${FILENAME} NAME)
  get_filename_component(TEST ${FILENAME} NAME_WE)
  add_executable(${TEST} ${SRC})
  add_dependencies(${TEST} csp)
  target_link_libraries(${TEST} csp)
#  target_link_libraries(${TEST} efence)
  # Add the test
  add_test(${TEST} ./${TEST})
  if(VALGRIND)
    # Add the valgrind test
    add_test(
      "${TEST}[valgrind]" ${VALGRIND} --leak-check=full --quiet --error-exitcode=1 ./${TEST}
    )
  endif()
endforeach()
