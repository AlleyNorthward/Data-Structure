file(
  GLOB_RECURSE
  LIB_SOURCES
  src/*.c
)

add_library(ds_lib STATIC ${LIB_SOURCES})

target_include_directories(
  ds_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
