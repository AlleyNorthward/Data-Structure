file(GLOB DEBUG_SOURCES src/*.c)
add_library(debug ${DEBUG_SOURCES})

target_include_directories(debug PUBLIC
    include
)











