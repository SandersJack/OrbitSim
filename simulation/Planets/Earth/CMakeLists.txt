include_directories(include)

file(GLOB SRC "src/*.cc")
file(GLOB INC "include/*.hh")


get_filename_component(SubDir ${CMAKE_CURRENT_SOURCE_DIR} NAME)
get_filename_component(ParentDir ${CMAKE_CURRENT_SOURCE_DIR} PATH)

ROOT_GENERATE_DICTIONARY(${SubDir}PersistencyDICT ${INC} OPTIONS -inlineInputHeader -Iinclude MODULE ${SubDir}Persistency LINKDEF ${SubDir}LinkDef.h )
add_library(${SubDir}Persistency SHARED ${SRC} ${SubDir}PersistencyDICT.cxx)
target_include_directories(${SubDir}Persistency PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

target_link_libraries(${SubDir}Persistency Parameters)
target_link_libraries(${SubDir}Persistency myLib)

message(LIB="${LIBRARY_OUTPUT_PATH}")
