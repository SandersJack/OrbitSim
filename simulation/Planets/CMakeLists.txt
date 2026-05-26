foreach(Planet ${PlanetsList})
  add_subdirectory(${Planet})
  target_link_libraries(OrbitSim ${Planet})
endforeach()
