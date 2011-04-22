#
#  Create Executables
#

add_executable( ImageReadStreamWrite ImageReadStreamWrite.cxx )
target_link_libraries( ImageReadStreamWrite ${ITK_LIBRARIES} )

add_executable( BinaryThresholdImageFilter BinaryThresholdImageFilter.cxx )
target_link_libraries( BinaryThresholdImageFilter ${ITK_LIBRARIES} )

add_executable( VotingBinaryHoleFillingImageFilter VotingBinaryHoleFillingImageFilter.cxx )
target_link_libraries( VotingBinaryHoleFillingImageFilter ${ITK_LIBRARIES} )

add_executable( SubtractImageFilter SubtractImageFilter.cxx )
target_link_libraries( SubtractImageFilter ${ITK_LIBRARIES} )

add_executable( ImageReadRegionOfInterestWrite ImageReadRegionOfInterestWrite.cxx )
target_link_libraries( ImageReadRegionOfInterestWrite ${ITK_LIBRARIES} )
