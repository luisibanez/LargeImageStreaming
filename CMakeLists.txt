cmake_minimum_required( VERSION 2.8 )

project( LargeImageStreaming )

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

include(CTest)
include(CPack)

set( CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/CMake"
  ${CMAKE_MODULE_PATH}
)

include_directories( BEFORE
  ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory( src )
add_subdirectory( tests )
add_subdirectory( documents )
