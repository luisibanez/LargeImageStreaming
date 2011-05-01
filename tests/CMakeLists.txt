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
find_path(LARGE_DATA_ROOT huc01resa.mhd DOC "Large datasets from Creatis")


if(LARGE_DATA_ROOT)

#
#   Read Write Profiling
#
add_test(NAME ReadWriteTest01
  COMMAND ImageReadStreamWrite
  ${LARGE_DATA_ROOT}/${INPUTFILENAME}.mhd
  ${TEMP}/ReadWriteTest01Output.mhd
  50 # Number of pieces to stream
  )

add_test(NAME ReadWriteTest02
  COMMAND ImageReadStreamWrite
  ${LARGE_DATA_ROOT}/${INPUTFILENAME}.mhd
  ${TEMP}/ReadWriteTest02Output.mhd
  100 # Number of pieces to stream
  )

add_test(NAME ReadWriteTest03
  COMMAND ImageReadStreamWrite
  ${LARGE_DATA_ROOT}/Shuc01resa.mhd
  ${TEMP}/ReadWriteTest03Output.mhd
  200 # Number of pieces to stream
  )


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


#
#  Here we define a macro that will move data through a pipeline
#
macro(PROCESS_DATA   INPUTFILENAME)

add_test(NAME ReadWriteTest_${INPUTFILENAME}
  COMMAND ImageReadStreamWrite
  ${LARGE_DATA_ROOT}/${INPUTFILENAME}.mhd
  ${TEMP}/ReadWriteTest_${INPUTFILENAME}.mhd
  50 # Number of pieces to stream
  )

add_test(NAME BinaryThresholdTest_${INPUTFILENAME}
  COMMAND BinaryThresholdImageFilter
  ${LARGE_DATA_ROOT}/${INPUTFILENAME}.mhd
  ${TEMP}/BinaryThresholdTest_${INPUTFILENAME}.mhd
  128 # Threshold value
  20  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest_01_${INPUTFILENAME}
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/BinaryThresholdTest_${INPUTFILENAME}.mhd
  ${TEMP}/VotingHoleFillingTest_01_${INPUTFILENAME}.mhd
  255 # Background (purposely using white here)
  0   # Foreground (purposely using black here)
  2   # Structuring element radius
  1   # Majority
  10  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest_02_${INPUTFILENAME}
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/VotingHoleFillingTest_01_${INPUTFILENAME}.mhd
  ${TEMP}/VotingHoleFillingTest_02_${INPUTFILENAME}.mhd
  0   # Background (purposely using black here)
  255 # Foreground (purposely using white here)
  2   # Structuring element radius
  1   # Majority
  10  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest_03_${INPUTFILENAME}
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/VotingHoleFillingTest_02_${INPUTFILENAME}.mhd
  ${TEMP}/VotingHoleFillingTest_03_${INPUTFILENAME}.mhd
  0   # Background (purposely using black here)
  255 # Foreground (purposely using white here)
  2   # Structuring element radius
  1   # Majority
  10  # Number of pieces to stream
  )

add_test(NAME VotingHoleFillingTest_04_${INPUTFILENAME}
  COMMAND VotingBinaryHoleFillingImageFilter
  ${TEMP}/VotingHoleFillingTest_03_${INPUTFILENAME}.mhd
  ${TEMP}/VotingHoleFillingTest_04_${INPUTFILENAME}.mhd
  0   # Background (purposely using black here)
  255 # Foreground (purposely using white here)
  2   # Structuring element radius
  1   # Majority
  10  # Number of pieces to stream
  )

add_test(NAME SubtractImageTest_${INPUTFILENAME}
  COMMAND SubtractImageFilter
  ${TEMP}/VotingHoleFillingTest_04_${INPUTFILENAME}.mhd
  ${TEMP}/VotingHoleFillingTest_01_${INPUTFILENAME}.mhd
  ${TEMP}/SubtractImageTest_${INPUTFILENAME}.mhd
  10  # Number of pieces to stream
  )

ExtractSlice( ${INPUTFILENAME}_001 ReadWriteTest_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_002 BinaryThresholdTest_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_003 VotingHoleFillingTest_01_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_004 VotingHoleFillingTest_02_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_005 VotingHoleFillingTest_03_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_006 VotingHoleFillingTest_04_${INPUTFILENAME} )
ExtractSlice( ${INPUTFILENAME}_007 SubtractImageTest_${INPUTFILENAME} )

endmacro(PROCESS_DATA)


PROCESS_DATA(huc01resa)
PROCESS_DATA(hunc34_14_a)


endif(LARGE_DATA_ROOT)
