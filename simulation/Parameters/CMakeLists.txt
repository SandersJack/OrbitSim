include_directories(include)

file(GLOB SRC "src/*.cc")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(Parameters STATIC ${SRC})
