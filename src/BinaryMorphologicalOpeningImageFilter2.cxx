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

#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

#include "itkTimeProbesCollectorBase.h"

int main(int argc, char * argv[])
{
  if( argc < 5 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " InputImage OutputImage Radius numberOfDataBlocks" << std::endl;
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

  typedef itk::BinaryBallStructuringElement< InputPixelType, Dimension > KernelType;
  KernelType ball;
  KernelType::SizeType ballSize;
  ballSize.Fill( atoi( argv[3] ) );
  ball.SetRadius( ballSize );
  ball.CreateStructuringElement();

  typedef itk::BinaryErodeImageFilter<  InputImageType, OutputImageType, KernelType > ErodeFilterType;
  typedef itk::BinaryDilateImageFilter< InputImageType, OutputImageType, KernelType > DilateFilterType;

  ErodeFilterType::Pointer eroder = ErodeFilterType::New();
  eroder->SetInput( reader->GetOutput() );
  eroder->SetKernel( ball );

  DilateFilterType::Pointer dilater = DilateFilterType::New();
  dilater->SetInput( eroder->GetOutput() );
  dilater->SetKernel( ball );

  itk::FilterStreamingWatcher watcher(eroder, "eroder");

  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( dilater->GetOutput() );
  writer->SetFileName( argv[2] );

  const unsigned int numberOfDataBlocks = atoi( argv[4] );

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
