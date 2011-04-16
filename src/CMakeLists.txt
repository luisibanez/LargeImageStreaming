#
#
#

add_executable( ImageReadStreamWrite ImageReadStreamWrite.cxx )
target_link_libraries( ImageReadStreamWrite ${ITK_LIBRARIES} )

add_executable( BinaryThresholdImageFilter BinaryThresholdImageFilter.cxx )
target_link_libraries( BinaryThresholdImageFilter ${ITK_LIBRARIES} )

add_executable( BinaryMorphologicalOpeningImageFilter BinaryMorphologicalOpeningImageFilter.cxx )
target_link_libraries( BinaryMorphologicalOpeningImageFilter ${ITK_LIBRARIES} )

add_executable( BinaryMorphologicalOpeningImageFilter2 BinaryMorphologicalOpeningImageFilter2.cxx )
target_link_libraries( BinaryMorphologicalOpeningImageFilter2 ${ITK_LIBRARIES} )

add_executable( VotingBinaryHoleFillingImageFilter VotingBinaryHoleFillingImageFilter.cxx )
target_link_libraries( VotingBinaryHoleFillingImageFilter ${ITK_LIBRARIES} )

add_executable( CavityDetectionImageFilter CavityDetectionImageFilter.cxx )
target_link_libraries( CavityDetectionImageFilter ${ITK_LIBRARIES} )
