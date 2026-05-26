include_directories(include)

file(GLOB SRC "src/*.cc")

get_filename_component(SubDir ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_library(${SubDir} STATIC ${SRC})
target_include_directories(${SubDir} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

target_link_libraries(${SubDir} Parameters)
target_link_libraries(${SubDir} myLib)
