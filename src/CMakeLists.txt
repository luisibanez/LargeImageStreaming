#
#
#

add_executable( ImageReadStreamWrite ImageReadStreamWrite.cxx )
target_link_libraries( ImageReadStreamWrite ${ITK_LIBRARIES} )

add_executable( BinaryThresholdImageFilter BinaryThresholdImageFilter.cxx )
target_link_libraries( BinaryThresholdImageFilter ${ITK_LIBRARIES} )

add_executable( BinaryMorphologicalOpeningImageFilter BinaryMorphologicalOpeningImageFilter.cxx )
target_link_libraries( BinaryMorphologicalOpeningImageFilter ${ITK_LIBRARIES} )
