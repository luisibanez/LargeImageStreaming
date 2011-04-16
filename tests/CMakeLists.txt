#
#
#
set(TEMP ${PROJECT_BINARY_DIR}/Testing/Temporary)
file(MAKE_DIRECTORY ${TEMP})

find_path(LARGE_DATA_ROOT merged.mhd DOC "Large datasets from Creatis")

if(LARGE_DATA_ROOT)

add_test(NAME ReadWriteTest01
  COMMAND ImageReadStreamWrite
  ${LARGE_DATA_ROOT}/merged.mhd
  ${TEMP}/ReadWriteTest01Output.mhd
  50 # Number of pieces to stream
  )

add_test(NAME ReadWriteTest02
  COMMAND ImageReadStreamWrite
  ${LARGE_DATA_ROOT}/merged.mhd
  ${TEMP}/ReadWriteTest02Output.mhd
  100 # Number of pieces to stream
  )

add_test(NAME ReadWriteTest03
  COMMAND ImageReadStreamWrite
  ${LARGE_DATA_ROOT}/merged.mhd
  ${TEMP}/ReadWriteTest03Output.mhd
  200 # Number of pieces to stream
  )

add_test(NAME BinaryThresholdTest01
  COMMAND BinaryThresholdImageFilter
  ${LARGE_DATA_ROOT}/merged.mhd
  ${TEMP}/BinaryThresholdTest01Output.mhd
  128 # Threshold value
  20  # Number of pieces to stream
  )

add_test(NAME BinaryOpeningTest01
  COMMAND BinaryMorphologicalOpeningImageFilter
  ${TEMP}/BinaryThresholdTest01Output.mhd
  ${TEMP}/BinaryMorphologicalOpeningImageFilterTest01Output.mhd
  2   # Structuring element radius
  20  # Number of pieces to stream
  )

add_test(NAME BinaryOpeningTest02
  COMMAND BinaryMorphologicalOpeningImageFilter2
  ${TEMP}/BinaryThresholdTest01Output.mhd
  ${TEMP}/BinaryMorphologicalOpeningImageFilter2Test01Output.mhd
  2   # Structuring element radius
  20  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest02
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/BinaryThresholdTest01Output.mhd
  ${TEMP}/VotingHoleFillingTest02Output.mhd
  2   # Structuring element radius
  1   # Majority
  20  # Number of pieces to stream
  )

endif(LARGE_DATA_ROOT)
