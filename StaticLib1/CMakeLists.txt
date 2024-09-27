set(PROJECT_NAME "StaticLib1")

set(CMAKE_GLOBAL_AUTOGEN_TARGET OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB StaticLib1_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  PREFIX Source
  FILES ${StaticLib1_SOURCE_FILES}
)

if(OS_WINDOWS)
  add_library(${PROJECT_NAME} STATIC ${StaticLib1_SOURCE_FILES})

  target_compile_definitions(${PROJECT_NAME} PRIVATE
	UNICODE
	_UNICODE
	_DEBUG
  )
endif() # OS_WINDOWS

# 
target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/../StaticLib2"
)

# build dependency
add_dependencies(${PROJECT_NAME}
  StaticLib2
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  StaticLib2
)