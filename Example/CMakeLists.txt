cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME "Example")
project(${PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create an empty source file as the rebuild trigger
# QCefView will update this file every time it gets built
file(WRITE ${CMAKE_BINARY_DIR}/auto_rebuild.cpp "/* Auto Rebuild Trigger */")

include_directories(
  ${CMAKE_SOURCE_DIR}/include
)

file(GLOB _SRC_FILES
  "*.h"
  "*.cpp"
  ${CMAKE_BINARY_DIR}/auto_rebuild.cpp
)

if(OS_WINDOWS)

  add_executable(${PROJECT_NAME} WIN32
    ${_SRC_FILES}
  )

  target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:CONSOLE")

  target_compile_definitions(${PROJECT_NAME} PRIVATE
    UNICODE
    _UNICODE
  )

  message(STATUS "VS_DEBUGGER_WORKING_DIRECTORY is: $<TARGET_FILE_DIR:${PROJECT_NAME}")
  set_target_properties(${PROJECT_NAME}
    PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
  )
  
  message(STATUS "CMAKE_CURRENT_SOURCE_DIR is: ${CMAKE_CURRENT_SOURCE_DIR}")
  target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../Dll1"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Dll2"
    "${CMAKE_CURRENT_SOURCE_DIR}/../StaticLib1"
    "${CMAKE_CURRENT_SOURCE_DIR}/../StaticLib2"
  )
  
  # build dependency
  add_dependencies(${PROJECT_NAME}
	Dll1
    Dll2
	StaticLib1
    StaticLib2
  )
  
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
	Dll1
    Dll2
	StaticLib1
    StaticLib2
  )

  add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD

    # Copy Dll binary to output folder
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:Dll1>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif() # OS_WINDOWS

message("CMAKE_CURRENT_BINARY_DIR is: ${CMAKE_CURRENT_BINARY_DIR}/output/$<CONFIG>/bin")
set_target_properties(${PROJECT_NAME}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/$<CONFIG>/bin
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/$<CONFIG>/lib
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/$<CONFIG>/lib
)