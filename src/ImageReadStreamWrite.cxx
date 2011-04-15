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

#include <iostream>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

int main(int argc, char *argv[])
{
  if ( argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile  outputImageFile numberOfDataBlocks" << std::endl;
    return EXIT_FAILURE;
    }

  typedef unsigned char                PixelType;
  const unsigned int Dimension = 3;

  typedef itk::Image<PixelType, Dimension>     ImageType;

  typedef itk::ImageFileReader< ImageType > ImageReaderType;
  typedef itk::ImageFileWriter< ImageType > ImageWriterType;

  ImageReaderType::Pointer reader = ImageReaderType::New();
  ImageWriterType::Pointer writer = ImageWriterType::New();

  std::string inputImageFileName  = argv[1];
  std::string outputImageFileName = argv[2];

  const unsigned int numberOfDataBlocks = atoi( argv[3] );

  reader->SetFileName( inputImageFileName );
  writer->SetFileName( outputImageFileName );

  writer->SetNumberOfStreamDivisions( numberOfDataBlocks );

  writer->SetInput( reader->GetOutput() );

  itk::SimpleFilterWatcher watcher(writer, "stream writing");


  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
