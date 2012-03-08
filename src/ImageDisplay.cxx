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

#include "itkImage.h"
#include "itkVTKImageExport.h"
#include "itkImageFileReader.h"

#include "vtkSmartPointer.h"
#include "vtkImageData.h"
#include "vtkImageImport.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImageCursor.h"

#include "itkReaderStreamingWatcher.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

template <typename ITK_Exporter, typename VTK_Importer>
void ConnectPipelines(ITK_Exporter exporter, VTK_Importer importer)
{
  importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
  importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
  importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
  importer->SetSpacingCallback(exporter->GetSpacingCallback());
  importer->SetOriginCallback(exporter->GetOriginCallback());
  importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
  importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
  importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
  importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
  importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
  importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
  importer->SetCallbackUserData(exporter->GetCallbackUserData());
}


int main(int argc, char * argv [] )
{

  // Load a scalar image using ITK and display it with VTK

  if( argc < 2 )
    {
    std::cerr << "Missing parameters" << std::endl;
    std::cerr << "Usage: " << argv[0] << " inputImageFileName " << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputImageFileName = argv[1];

  itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(
    inputImageFileName.c_str(), itk::ImageIOFactory::ReadMode);

  imageIO->SetFileName( inputImageFileName );
  imageIO->ReadImageInformation();

  std::cout << "File = " << inputImageFileName << std::endl;
  std::cout << "File Recognized by " << imageIO->GetNameOfClass() << std::endl;


  itk::ImageIOBase::IOComponentType componentType = imageIO->GetComponentType();

  const unsigned int ImageDimension = 3;

  try
    {

    //
    //   Pixel Type = unsigned char
    //
    typedef unsigned char PixelType1;
    typedef itk::Image< PixelType1, ImageDimension > ImageType1;
    typedef itk::ImageFileReader< ImageType1 > ReaderType1;
    ReaderType1::Pointer reader1  = ReaderType1::New();
    reader1->SetFileName( inputImageFileName );

    typedef itk::VTKImageExport< ImageType1 > ExportFilterType1;
    ExportFilterType1::Pointer exporter1 = ExportFilterType1::New();

    exporter1->SetInput( reader1->GetOutput() );
    itk::ReaderStreamingWatcher watcher1( reader1 );

    //
    //   Pixel Type = float
    //
    typedef float PixelType2;
    typedef itk::Image< PixelType2, ImageDimension > ImageType2;
    typedef itk::ImageFileReader< ImageType2 > ReaderType2;
    ReaderType2::Pointer reader2  = ReaderType2::New();
    reader2->SetFileName( inputImageFileName );

    typedef itk::VTKImageExport< ImageType2 > ExportFilterType2;
    ExportFilterType2::Pointer exporter2 = ExportFilterType2::New();

    exporter2->SetInput( reader2->GetOutput() );
    itk::ReaderStreamingWatcher watcher2( reader2 );

    //
    //   Pixel Type = signed short
    //
    typedef signed short PixelType3;
    typedef itk::Image< PixelType3, ImageDimension > ImageType3;
    typedef itk::ImageFileReader< ImageType3 > ReaderType3;
    ReaderType3::Pointer reader3  = ReaderType3::New();
    reader3->SetFileName( inputImageFileName );

    typedef itk::VTKImageExport< ImageType3 > ExportFilterType3;
    ExportFilterType3::Pointer exporter3 = ExportFilterType3::New();

    exporter3->SetInput( reader3->GetOutput() );
    itk::ReaderStreamingWatcher watcher3( reader3 );


    //
    //  Setup the rest of the pipeline
    //
    VTK_CREATE( vtkImageImport, vtkImporter );


    switch( componentType )
      {
      case itk::ImageIOBase::UCHAR:
        {
        ConnectPipelines(exporter1, vtkImporter);
        break;
        }
      case itk::ImageIOBase::FLOAT:
        {
        ConnectPipelines(exporter2, vtkImporter);
        break;
        }
      case itk::ImageIOBase::CHAR:
      case itk::ImageIOBase::USHORT:
      case itk::ImageIOBase::SHORT:
        {
        ConnectPipelines(exporter3, vtkImporter);
        break;
        }
      case itk::ImageIOBase::ULONG:
      case itk::ImageIOBase::LONG:
      case itk::ImageIOBase::UINT:
      case itk::ImageIOBase::INT:
      case itk::ImageIOBase::DOUBLE:
      case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
        {
        std::cerr << "Unsupported pixel type in this application." << std::endl;
        return EXIT_FAILURE;
        }
      }

    vtkImporter->UpdateInformation();

    int * extent = vtkImporter->GetOutput()->GetWholeExtent();

    int slice_min = extent[2];
    int slice_max = extent[2 + 1];

    //------------------------------------------------------------------------
    // VTK visualization pipeline
    //------------------------------------------------------------------------

    VTK_CREATE( vtkImageActor, actor );
    VTK_CREATE( vtkRenderer, renderer );
    VTK_CREATE( vtkRenderWindow, renWin );
    VTK_CREATE( vtkRenderWindowInteractor, iren );
    VTK_CREATE( vtkInteractorStyleImageCursor, interactorStyle );

    actor->SetInput(vtkImporter->GetOutput());

    int middleSlice = ( slice_min + slice_max ) / 2.0;

    actor->SetDisplayExtent(
        extent[0], extent[1], extent[2], extent[3], middleSlice, middleSlice );

    actor->SetInterpolate(0);

    interactorStyle->SetImageActor( actor );
    interactorStyle->SetRenderWindow( renWin );

    renWin->SetSize(500, 500);
    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);
    iren->SetInteractorStyle( interactorStyle );

    renderer->AddActor(actor);
    renderer->SetBackground(0.4392, 0.5020, 0.5647);

    renWin->Render();
    iren->Start();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception catched !! " << e << std::endl;
    }

  return EXIT_SUCCESS;
}
