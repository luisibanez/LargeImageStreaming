cmake_minimum_required( VERSION 2.8 )

project( LargeImageStreaming )

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

option(USE_VTK "Build VTK based viewer ." OFF)

if( USE_VTK )
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
endif()

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

option(BUILD_REPORTS "Build Reproducible Reports." OFF)

if( BUILD_REPORTS )
  add_subdirectory( documents )
endif()
