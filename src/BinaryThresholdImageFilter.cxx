/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkBinaryThresholdImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFilterStreamingWatcher.h"

#include "itkTimeProbesCollectorBase.h"

int main( int argc, char * argv[] )
{
  if( argc < 5 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputImageFile ";
    std::cerr << " thresholdValue numberOfDataBlocks" << std::endl;
    return EXIT_FAILURE;
    }

  typedef  unsigned char  InputPixelType;
  typedef  unsigned char  OutputPixelType;

  const unsigned int Dimension = 3;

  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;

  typedef itk::BinaryThresholdImageFilter<
               InputImageType, OutputImageType >  FilterType;

  typedef itk::ImageFileReader< InputImageType >  ReaderType;

  typedef itk::ImageFileWriter< OutputImageType >  WriterType;


  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  reader->SetFileName( argv[1] );


  filter->SetInput( reader->GetOutput() );

  const OutputPixelType outsideValue = 0;
  const OutputPixelType insideValue  = 255;

  filter->SetOutsideValue( outsideValue );
  filter->SetInsideValue(  insideValue  );

  const InputPixelType lowerThreshold = atoi( argv[3] );
  const InputPixelType upperThreshold = itk::NumericTraits< InputPixelType >::max();

  filter->SetLowerThreshold( lowerThreshold );
  filter->SetUpperThreshold( upperThreshold );

  itk::FilterStreamingWatcher watcher(filter, "thresholding");

  writer->SetFileName( argv[2] );

  const unsigned int numberOfDataBlocks = atoi( argv[4] );

  writer->SetNumberOfStreamDivisions( numberOfDataBlocks );

  itk::TimeProbesCollectorBase chronometer;

  chronometer.Start("Filtering");

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  chronometer.Stop("Filtering");
  chronometer.Report( std::cout );

  return EXIT_SUCCESS;
}
