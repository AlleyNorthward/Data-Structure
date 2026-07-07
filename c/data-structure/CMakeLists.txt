file(
  GLOB_RECURSE
  LIB_SOURCES
  src/*.c
)

add_library(ds_lib STATIC ${LIB_SOURCES})

target_include_directories(
  ds_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/list
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/array
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stack
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/index_list
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/list
)
