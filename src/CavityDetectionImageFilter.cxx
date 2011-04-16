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
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFilterStreamingWatcher.h"

#include "itkVotingBinaryIterativeHoleFillingImageFilter.h"
#include "itkSubtractImageFilter.h"

#include "itkTimeProbesCollectorBase.h"

int main(int argc, char * argv[])
{
  if( argc < 9 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " InputImage OutputImage Background Foreground Radius Majority MaximumIterations numberOfDataBlocks" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;

  typedef unsigned char       InputPixelType;
  typedef unsigned char       OutputPixelType;

  typedef itk::Image< InputPixelType, Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >  OutputImageType;

  typedef itk::ImageFileReader< InputImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  InputImageType::SizeType neighborhoodRadius;
  neighborhoodRadius[0] = atoi( argv[5] );
  neighborhoodRadius[1] = atoi( argv[5] );
  neighborhoodRadius[2] = atoi( argv[5] );

  typedef itk::VotingBinaryIterativeHoleFillingImageFilter< InputImageType > VotingFilterType;

  VotingFilterType::Pointer filter = VotingFilterType::New();
  filter->SetInput( reader->GetOutput() );

  typedef itk::SubtractImageFilter<
    InputImageType, InputImageType, OutputImageType > SubtractFilterType;

  SubtractFilterType::Pointer difference = SubtractFilterType::New();
  difference->SetInput1( filter->GetOutput() );
  difference->SetInput2( reader->GetOutput() );

  // Set to eliminate isolated bright spots
  // Using black as the foreground.
  filter->SetBackgroundValue( atoi( argv[3] ) );
  filter->SetForegroundValue( atoi( argv[4] ) );

  filter->SetRadius( neighborhoodRadius );

  filter->SetMajorityThreshold( atoi( argv[6] ) );

  filter->SetMaximumNumberOfIterations( atoi( argv[7] ) );

  itk::FilterStreamingWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( difference->GetOutput() );
  writer->SetFileName( argv[2] );

  const unsigned int numberOfDataBlocks = atoi( argv[8] );

  writer->SetNumberOfStreamDivisions( numberOfDataBlocks );

  itk::TimeProbesCollectorBase chronometer;

  chronometer.Start("Filtering");

  try
    {
    writer->Update();
    }
  catch ( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  chronometer.Stop("Filtering");
  chronometer.Report( std::cout );

  return EXIT_SUCCESS;
}
