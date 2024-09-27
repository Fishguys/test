project(Dll1)

set(CMAKE_GLOBAL_AUTOGEN_TARGET OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB Dll1_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  PREFIX Source
  FILES ${Dll1_SOURCE_FILES}
)

if(OS_WINDOWS)
  add_library(Dll1 SHARED ${Dll1_SOURCE_FILES})
endif() # OS_WINDOWS

# 
target_include_directories(Dll1 PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/../Dll2"
)

# build dependency
add_dependencies(Dll1
  Dll2
)

target_link_libraries(Dll1
  PRIVATE
  Dll2
)