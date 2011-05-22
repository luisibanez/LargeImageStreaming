PROJECT(LargeImageStreamingReport001)

FIND_PACKAGE(LATEX)

IF(NOT PDFLATEX_COMPILER)
  MESSAGE("pdflatex compiler was not found. Please pass to advanced mode and provide its full path")
ELSE(NOT PDFLATEX_COMPILER)


FIND_PROGRAM(FIG2DEV_EXECUTABLE NAMES fig2dev)


#
# These are source files needed for the report generation.
#
SET(REPORT_LATEX_SRCS
  algorithmic.sty
  algorithm.sty
  amssymb.sty
  fancyhdr.sty
  floatflt.sty
  fncychap.sty
  InsightArticle.cls
  InsightJournal.bib
  InsightJournal.ist
  InsightJournal.sty
  times.sty
  picins.sty
  )

#
# This adds a custom command for each source file in REPORT_LATEX_SRCS
# that copies the file from the source directory to the binary
# directory where the pdf will be generated.
#
FOREACH(SOURCE_FILE ${REPORT_LATEX_SRCS})
   ADD_CUSTOM_COMMAND(
     OUTPUT   ${LargeImageStreamingReport001_BINARY_DIR}/${SOURCE_FILE}
     DEPENDS  ${LargeImageStreamingReport001_SOURCE_DIR}/../LaTeX/${SOURCE_FILE}
     COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LargeImageStreamingReport001_SOURCE_DIR}/../LaTeX/${SOURCE_FILE}
        ${LargeImageStreamingReport001_BINARY_DIR}/${SOURCE_FILE}
    )
   SET(COPY_RESULTS ${COPY_RESULTS} ${LargeImageStreamingReport001_BINARY_DIR}/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE ${REPORT_LATEX_SRCS})


SET(REPORT_SRCS
  LargeImageStreamingReport001.tex
  ITKStreaming-001.fig
  ITKStreaming-002.fig
  ITKStreaming-003.fig
  ../../src/ImageReadStreamWrite.cxx
  ../../src/BinaryThresholdImageFilter.cxx
  ../../src/VotingBinaryHoleFillingImageFilter.cxx
  )

#
# This adds a custom command for each source file in REPORT_SRCS
# that copies the file from the source directory to the binary
# directory where the pdf will be generated.
#
FOREACH(SOURCE_FILE ${REPORT_SRCS})
   ADD_CUSTOM_COMMAND(
     OUTPUT   ${LargeImageStreamingReport001_BINARY_DIR}/${SOURCE_FILE}
     DEPENDS  ${LargeImageStreamingReport001_SOURCE_DIR}/${SOURCE_FILE}
     COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LargeImageStreamingReport001_SOURCE_DIR}/${SOURCE_FILE}
        ${LargeImageStreamingReport001_BINARY_DIR}/${SOURCE_FILE}
    )
   SET(COPY_RESULTS ${COPY_RESULTS} ${LargeImageStreamingReport001_BINARY_DIR}/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE ${REPORT_SRCS})

FILE(GLOB XFIG_FIGURES *.fig)

FOREACH(image ${XFIG_FIGURES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  ADD_CUSTOM_COMMAND(
    OUTPUT    ${PROJECT_BINARY_DIR}/${ImageBase}.pdf
    DEPENDS   ${PROJECT_BINARY_DIR}/${ImageBase}.fig
    COMMAND   ${FIG2DEV_EXECUTABLE}
      -L pdf ${PROJECT_BINARY_DIR}/${ImageBase}.fig ${PROJECT_BINARY_DIR}/${ImageBase}.pdf
    )
  SET(EPS_DEPS ${EPS_DEPS} ${PROJECT_BINARY_DIR}/${ImageBase}.pdf)
ENDFOREACH(image)


SET(REPORT_ELEMENTS
  ${EPS_DEPS}
  )

#
# This adds a custom target that generates LargeImageStreamingReport001.pdf
# This target depends on the list of copied files created
# with the custom command above and the Plots target.
#
ADD_CUSTOM_TARGET(Report001 ALL
   COMMAND ${PDFLATEX_COMPILER}
        ${LargeImageStreamingReport001_SOURCE_DIR}/LargeImageStreamingReport001.tex
        -output-directory ${LargeImageStreamingReport001_BINARY_DIR}
   COMMAND ${PDFLATEX_COMPILER}
        ${LargeImageStreamingReport001_SOURCE_DIR}/LargeImageStreamingReport001.tex
        -output-directory ${LargeImageStreamingReport001_BINARY_DIR}
   DEPENDS ${COPY_RESULTS} ${REPORT_ELEMENTS}
   WORKING_DIRECTORY ${LargeImageStreamingReport001_BINARY_DIR}
   )

ENDIF(NOT PDFLATEX_COMPILER)
