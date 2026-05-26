cmake_minimum_required(VERSION 3.20)

include(CMakePrintHelpers)
include(FetchContent)

project(OrbitSim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(HDF5 REQUIRED COMPONENTS C)
message(STATUS "Found HDF5 ${HDF5_VERSION} at ${HDF5_INCLUDE_DIRS}")

FetchContent_Declare(
    HighFive
    GIT_REPOSITORY https://github.com/BlueBrain/HighFive.git
    GIT_TAG v2.10.0
)
set(HIGHFIVE_USE_BOOST OFF CACHE BOOL "" FORCE)
set(HIGHFIVE_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(HIGHFIVE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(HIGHFIVE_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(HighFive)

set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/")
set(EXECUTABLE_OUTPUT_PATH ".")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

file(GLOB SRC "src/*.cc")
set(MAIN_FILE Orbits.cc)
include_directories(include)

add_executable(OrbitSim ${MAIN_FILE} ${SRC})

add_library(myLib STATIC src/Vector3D.cc src/Planets.cc src/Moons.cc src/Satellite.cc)
target_link_libraries(${PROJECT_NAME} myLib)
target_link_libraries(${PROJECT_NAME} HighFive)

include_directories("Parameters/include")
target_link_libraries(${PROJECT_NAME} Parameters)
add_subdirectory(Parameters)

set(PlanetsList Earth Mars Jupiter Saturn Uranus Neptune Venus Mercury)
set(PlanetsIncludeDirs ${PlanetsList})
list(TRANSFORM PlanetsIncludeDirs PREPEND "Planets/")
list(TRANSFORM PlanetsIncludeDirs APPEND "/include")
include_directories(${PlanetsIncludeDirs})

add_subdirectory(Planets)

message(STATUS "Building Planets: ")
foreach(Planet ${PlanetsList})
    message(STATUS "${Planet}")
endforeach()
