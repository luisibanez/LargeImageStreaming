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
#include "itkImageIOFactory.h"
#include "itkImageIOFactoryRegisterManager.h"

int main(int argc, char *argv[])
{
  if ( argc < 2 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile  " << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputImageFileName  = argv[1];

  itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(
       inputImageFileName.c_str(), itk::ImageIOFactory::ReadMode);


  if ( imageIO.IsNull() )
    {
    std::cerr << " Could not create IO object for file " << std::endl;
    std::cerr << inputImageFileName.c_str() << std::endl;
    std::cerr << std::endl;
    std::cerr << "  Tried to create one of the following:" << std::endl;

    std::list< itk::LightObject::Pointer > allobjects =
        itk::ObjectFactoryBase::CreateAllInstance("itkImageIOBase");

    std::list< itk::LightObject::Pointer >::iterator imageIOItr = allobjects.begin();

    while( imageIOItr != allobjects.end() )
        {
        itk::ImageIOBase *io = dynamic_cast< itk::ImageIOBase * >( imageIOItr->GetPointer() );
        std::cerr << "    " << io->GetNameOfClass() << std::endl;
        }

    std::cerr << "  You probably failed to set a file suffix, or" << std::endl;
    std::cerr << "    set the suffix to an unsupported type." << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "File = " << inputImageFileName << std::endl;

  std::cout << "File Recognized by " << imageIO->GetNameOfClass() << std::endl;

  imageIO->SetFileName( inputImageFileName );
  imageIO->ReadImageInformation();

  const unsigned int numberOfDimensionsIO = imageIO->GetNumberOfDimensions();

  std::cout << "Image Dimension = " << numberOfDimensionsIO << std::endl;

  std::cout << "Size = ";

  for( unsigned int i = 0; i < numberOfDimensionsIO; i++ )
    {
    std::cout << imageIO->GetDimensions(i) << " ";
    }
  std::cout << std::endl;


  std::cout << "Spacing = ";

  for( unsigned int i = 0; i < numberOfDimensionsIO; i++ )
    {
    std::cout << imageIO->GetSpacing(i) << " ";
    }
  std::cout << std::endl;


  std::cout << "Origin = ";

  for( unsigned int i = 0; i < numberOfDimensionsIO; i++ )
    {
    std::cout << imageIO->GetOrigin(i) << " ";
    }
  std::cout << std::endl;

  std::cout << "Number of Components = ";
  std::cout << imageIO->GetNumberOfComponents() << std::endl;

  std::cout << "Orientation = " << std::endl;

  for( unsigned int i = 0; i < numberOfDimensionsIO; i++ )
    {
    for( unsigned int j = 0; j < numberOfDimensionsIO; j++ )
      {
      std::cout << imageIO->GetDirection(i)[j] << " ";
      }
    std::cout << std::endl;
    }
  std::cout << std::endl;

  std::cout << std::endl;

  return EXIT_SUCCESS;
}
