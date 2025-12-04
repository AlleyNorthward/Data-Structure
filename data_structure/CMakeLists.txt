file(GLOB DATA_STRUCTURE_SOURCES src/*.c)
add_library(data_structure ${DATA_STRUCTURE_SOURCES})

target_include_directories(data_structure PUBLIC
    include
)











