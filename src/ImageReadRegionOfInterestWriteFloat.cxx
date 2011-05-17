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
#include "itkRegionOfInterestImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImage.h"

int main( int argc, char ** argv )
{

  if( argc < 8 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    std::cerr << " SliceNumberInZ startX startY sizeX sizeY" << std::endl;
    return EXIT_FAILURE;
    }


  typedef float               InputPixelType;
  typedef unsigned char       OutputPixelType;
  const   unsigned int        Dimension = 3;

  typedef itk::Image< InputPixelType,  Dimension >    InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >    OutputImageType;

  typedef itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  typedef itk::RegionOfInterestImageFilter< InputImageType,
                                            InputImageType > ExtractFilterType;

  typedef itk::RescaleIntensityImageFilter< InputImageType,
                                            OutputImageType > RescaleFilterType;

  ExtractFilterType::Pointer extract = ExtractFilterType::New();
  RescaleFilterType::Pointer rescale = RescaleFilterType::New();

  OutputImageType::IndexType start;
  start[0] = atoi( argv[4] );
  start[1] = atoi( argv[5] );
  start[2] = atoi( argv[3] );


  OutputImageType::SizeType size;
  size[0] = atoi( argv[6] );
  size[1] = atoi( argv[7] );
  size[2] = 1; // one slice in Z

  OutputImageType::RegionType desiredRegion;
  desiredRegion.SetSize(  size  );
  desiredRegion.SetIndex( start );

  extract->SetRegionOfInterest( desiredRegion );

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename );

  extract->SetInput( reader->GetOutput() );
  rescale->SetInput( extract->GetOutput() );
  writer->SetInput( rescale->GetOutput() );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
