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

add_test(NAME VotingHoleFillingTest02
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/BinaryThresholdTest01Output.mhd
  ${TEMP}/VotingHoleFillingTest02Output.mhd
  255 # Background (purposely using white here)
  0   # Foreground (purposely using black here)
  2   # Structuring element radius
  1   # Majority
  10  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest03
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/VotingHoleFillingTest02Output.mhd
  ${TEMP}/VotingHoleFillingTest03Output.mhd
  0   # Background (purposely using black here)
  255 # Foreground (purposely using white here)
  2   # Structuring element radius
  1   # Majority
  10  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest04
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/VotingHoleFillingTest03Output.mhd
  ${TEMP}/VotingHoleFillingTest04Output.mhd
  0   # Background (purposely using black here)
  255 # Foreground (purposely using white here)
  2   # Structuring element radius
  1   # Majority
  10  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest05
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/VotingHoleFillingTest04Output.mhd
  ${TEMP}/VotingHoleFillingTest05Output.mhd
  0   # Background (purposely using black here)
  255 # Foreground (purposely using white here)
  2   # Structuring element radius
  1   # Majority
  10  # Number of pieces to stream
  )

add_test(NAME SubtractImageTest01
  COMMAND SubtractImageFilter
  ${TEMP}/VotingHoleFillingTest05Output.mhd
  ${TEMP}/VotingHoleFillingTest02Output.mhd
  ${TEMP}/SubtractImageTest01Output.mhd
  10  # Number of pieces to stream
  )


#
# Macro to extract slices from the datasets
# Typically these slices will be used in the
# technical report
#
macro(ExtractSlice TEST_NUMBER IMAGE_BASE_NAME)
add_test(NAME SliceTest${TEST_NUMBER}
  COMMAND ImageReadRegionOfInterestWrite
  ${TEMP}/${IMAGE_BASE_NAME}.mhd
  ${TEMP}/${IMAGE_BASE_NAME}_Slice.png
  800  # Central slice along Z
  0    # Start index in X
  0    # Start index in Y
  2048 # Size in pixels along X
  2048 # Size in pixels along Y
  )
endmacro()

ExtractSlice(001 ReadWriteTest02Output)
ExtractSlice(002 BinaryThresholdTest01Output)
ExtractSlice(003 VotingHoleFillingTest02Output)
ExtractSlice(004 VotingHoleFillingTest03Output)
ExtractSlice(005 VotingHoleFillingTest04Output)
ExtractSlice(006 VotingHoleFillingTest05Output)
ExtractSlice(007 SubtractImageTest01Output)

endif(LARGE_DATA_ROOT)
