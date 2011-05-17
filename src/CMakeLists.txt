#
#  Create Executables
#

add_executable( ImageReadStreamWrite ImageReadStreamWrite.cxx )
target_link_libraries( ImageReadStreamWrite ${ITK_LIBRARIES} )

add_executable( ImageFloatReadStreamWrite ImageFloatReadStreamWrite.cxx )
target_link_libraries( ImageFloatReadStreamWrite ${ITK_LIBRARIES} )

add_executable( BinaryThresholdImageFilter BinaryThresholdImageFilter.cxx )
target_link_libraries( BinaryThresholdImageFilter ${ITK_LIBRARIES} )

add_executable( BinaryThresholdFloatImageFilter BinaryThresholdFloatImageFilter.cxx )
target_link_libraries( BinaryThresholdFloatImageFilter ${ITK_LIBRARIES} )

add_executable( VotingBinaryHoleFillingImageFilter VotingBinaryHoleFillingImageFilter.cxx )
target_link_libraries( VotingBinaryHoleFillingImageFilter ${ITK_LIBRARIES} )

add_executable( SubtractImageFilter SubtractImageFilter.cxx )
target_link_libraries( SubtractImageFilter ${ITK_LIBRARIES} )

add_executable( ImageReadRegionOfInterestWrite ImageReadRegionOfInterestWrite.cxx )
target_link_libraries( ImageReadRegionOfInterestWrite ${ITK_LIBRARIES} )

add_executable( ImageReadRegionOfInterestWriteFloat ImageReadRegionOfInterestWriteFloat.cxx )
target_link_libraries( ImageReadRegionOfInterestWriteFloat ${ITK_LIBRARIES} )

add_executable( ImageReadPrint ImageReadPrint.cxx )
target_link_libraries( ImageReadPrint ${ITK_LIBRARIES} )

if( USE_VTK )
  add_executable( ImageDisplay ImageDisplay.cxx vtkInteractorStyleImageCursor.cxx )
  target_link_libraries( ImageDisplay ${ITK_LIBRARIES}
    vtkRendering vtkIO vtkHybrid )

  add_executable( ImageSurfaceDisplay ImageSurfaceDisplay.cxx )
  target_link_libraries( ImageSurfaceDisplay ${ITK_LIBRARIES}
    vtkRendering vtkIO vtkHybrid )
endif()
