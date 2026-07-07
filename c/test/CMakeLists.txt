file(
  GLOB_RECURSE
  TEST_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)


foreach(test_src ${TEST_FILES})
  get_filename_component(target_name ${test_src} NAME_WE)
  
  add_executable(${target_name} ${test_src})
  target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${target_name} PRIVATE ds_lib)
endforeach()
