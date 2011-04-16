#
#
#
set(TEMP ${PROJECT_BINARY_DIR}/tests/Temporary)
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
  ${TEMP}/ReadWriteTest01Output.mhd
  100 # Number of pieces to stream
  )

add_test(NAME ReadWriteTest03
  COMMAND ImageReadStreamWrite
  ${LARGE_DATA_ROOT}/merged.mhd
  ${TEMP}/ReadWriteTest01Output.mhd
  200 # Number of pieces to stream
  )

endif(LARGE_DATA_ROOT)

