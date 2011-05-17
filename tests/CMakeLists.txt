#
# This tests will process data through a streaming pipeline
#
set(TEMP ${PROJECT_BINARY_DIR}/Testing/Temporary)
file(MAKE_DIRECTORY ${TEMP})

#
#
#   These data sets can be obtained from:
#
#      http://www.insight-journal.org/midas/community/view/33
#      http://hdl.handle.net/1926/1750
#
#
find_path(LARGE_DATA_ROOT hunc34_14_a.mhd DOC "Large datasets from Creatis")


if(LARGE_DATA_ROOT)

#
# Macro to extract slices from the datasets
# Typically these slices will be used in the
# technical report
#
macro(ExtractSlice TEST_ID IMAGE_BASE_NAME)
add_test(NAME SliceTest_${TEST_ID}
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

macro(ExtractSliceFloat TEST_ID IMAGE_BASE_NAME)
add_test(NAME SliceTest_${TEST_ID}
  COMMAND ImageReadRegionOfInterestWriteFloat
  ${TEMP}/${IMAGE_BASE_NAME}.mhd
  ${TEMP}/${IMAGE_BASE_NAME}_Slice.png
  800  # Central slice along Z
  0    # Start index in X
  0    # Start index in Y
  2048 # Size in pixels along X
  2048 # Size in pixels along Y
  )
endmacro()


#
#   Read Write Profiling
#
macro(STREAM_FLOAT_DATA   INPUTFILENAME CHUNKS)

add_test(NAME ReadWriteTest_${INPUTFILENAME}
  COMMAND ImageFloatReadStreamWrite
  ${LARGE_DATA_ROOT}/${INPUTFILENAME}.mhd
  ${TEMP}/ReadWriteTest_${INPUTFILENAME}.mhd
  ${CHUNKS} # Number of pieces to stream
  )

endmacro(STREAM_FLOAT_DATA)

macro(STREAM_DATA   INPUTFILENAME CHUNKS)

add_test(NAME ReadWriteTest_${INPUTFILENAME}
  COMMAND ImageReadStreamWrite
  ${LARGE_DATA_ROOT}/${INPUTFILENAME}.mhd
  ${TEMP}/ReadWriteTest_${INPUTFILENAME}.mhd
  ${CHUNKS} # Number of pieces to stream
  )

endmacro(STREAM_DATA)


macro(BINARIZE_FLOAT_DATA   INPUTFILENAME CHUNKS)

add_test(NAME BinaryThresholdTest_${INPUTFILENAME}
  COMMAND BinaryThresholdFloatImageFilter
  ${LARGE_DATA_ROOT}/${INPUTFILENAME}.mhd
  ${TEMP}/BinaryThresholdTest_${INPUTFILENAME}.mhd
  128 # Threshold value
  ${CHUNKS}  # Number of pieces to stream
  )

endmacro(BINARIZE_FLOAT_DATA)


macro(BINARIZE_CHAR_DATA   INPUTFILENAME CHUNKS)

add_test(NAME BinaryThresholdTest_${INPUTFILENAME}
  COMMAND BinaryThresholdImageFilter
  ${LARGE_DATA_ROOT}/${INPUTFILENAME}.mhd
  ${TEMP}/BinaryThresholdTest_${INPUTFILENAME}.mhd
  128 # Threshold value
  ${CHUNKS}  # Number of pieces to stream
  )

endmacro(BINARIZE_CHAR_DATA)



macro(PROCESS_DATA   INPUTFILENAME CHUNKS)

ExtractSlice( ${INPUTFILENAME}_002 BinaryThresholdTest_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_003 VotingHoleFillingTest_01_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_004 VotingHoleFillingTest_02_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_005 VotingHoleFillingTest_03_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_006 VotingHoleFillingTest_04_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_007 SubtractImageTest_${INPUTFILENAME} )

add_test(NAME SubtractImageTest_${INPUTFILENAME}
  COMMAND SubtractImageFilter
  ${TEMP}/VotingHoleFillingTest_04_${INPUTFILENAME}.mhd
  ${TEMP}/VotingHoleFillingTest_01_${INPUTFILENAME}.mhd
  ${TEMP}/SubtractImageTest_${INPUTFILENAME}.mhd
  ${CHUNKS}  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest_04_${INPUTFILENAME}
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/VotingHoleFillingTest_03_${INPUTFILENAME}.mhd
  ${TEMP}/VotingHoleFillingTest_04_${INPUTFILENAME}.mhd
  0   # Background (purposely using black here)
  255 # Foreground (purposely using white here)
  2   # Structuring element radius
  1   # Majority
  ${CHUNKS}  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest_03_${INPUTFILENAME}
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/VotingHoleFillingTest_02_${INPUTFILENAME}.mhd
  ${TEMP}/VotingHoleFillingTest_03_${INPUTFILENAME}.mhd
  0   # Background (purposely using black here)
  255 # Foreground (purposely using white here)
  2   # Structuring element radius
  1   # Majority
  ${CHUNKS}  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest_02_${INPUTFILENAME}
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/VotingHoleFillingTest_01_${INPUTFILENAME}.mhd
  ${TEMP}/VotingHoleFillingTest_02_${INPUTFILENAME}.mhd
  0   # Background (purposely using black here)
  255 # Foreground (purposely using white here)
  2   # Structuring element radius
  1   # Majority
  ${CHUNKS}  # Number of pieces to stream
  )


add_test(NAME VotingHoleFillingTest_01_${INPUTFILENAME}
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/BinaryThresholdTest_${INPUTFILENAME}.mhd
  ${TEMP}/VotingHoleFillingTest_01_${INPUTFILENAME}.mhd
  255 # Background (purposely using white here)
  0   # Foreground (purposely using black here)
  2   # Structuring element radius
  1   # Majority
  ${CHUNKS}  # Number of pieces to stream
  )

endmacro(PROCESS_DATA)

PROCESS_DATA(hunc34_14_a 6)
PROCESS_DATA(hunc34_14_a_float 20)

ExtractSlice( hunc34_14_a_001 ReadWriteTest_hunc34_14_a )
ExtractSliceFloat( hunc34_14_a_float_001 ReadWriteTest_hunc34_14_a_float )

BINARIZE_CHAR_DATA(hunc34_14_a 6)
BINARIZE_FLOAT_DATA(hunc34_14_a_float 20)

STREAM_DATA(hunc34_14_a 6)
STREAM_FLOAT_DATA(hunc34_14_a_float 20)

endif(LARGE_DATA_ROOT)
