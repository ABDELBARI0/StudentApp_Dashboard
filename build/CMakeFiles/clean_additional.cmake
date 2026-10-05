# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\StudentApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\StudentApp_autogen.dir\\ParseCache.txt"
  "StudentApp_autogen"
  )
endif()
