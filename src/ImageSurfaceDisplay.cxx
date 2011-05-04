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
#include "vtkActor.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkContourFilter.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"

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

  if( argc < 3 )
    {
    std::cerr << "Missing parameters" << std::endl;
    std::cerr << "Usage: " << argv[0] << " inputImageFilename isolevel  [-I] [-Screenshot outpu.png]" << std::endl;
    return EXIT_FAILURE;
    }

  // Run the interactor event loop if -I is specified.
  bool interactive = false;
  for (int i = 0; i < argc; i++)
    {
    if (strcmp("-I", argv[i]) == 0)
      {
      interactive = true;
      }
    }

  try
    {
    typedef unsigned char PixelType;

    typedef itk::Image< PixelType, 3 > ImageType;

    typedef itk::ImageFileReader< ImageType > ReaderType;

    ReaderType::Pointer reader  = ReaderType::New();

    reader->SetFileName( argv[1] );

    typedef itk::VTKImageExport< ImageType > ExportFilterType;
    ExportFilterType::Pointer itkExporter = ExportFilterType::New();

    itkExporter->SetInput( reader->GetOutput() );

    itk::ReaderStreamingWatcher watcher( reader );

    VTK_CREATE( vtkImageImport, vtkImporter );

    ConnectPipelines(itkExporter, vtkImporter);

    vtkImporter->UpdateInformation();

    //------------------------------------------------------------------------
    // VTK visualization pipeline
    //------------------------------------------------------------------------

    VTK_CREATE( vtkActor, actor );
    VTK_CREATE( vtkRenderer, renderer );
    VTK_CREATE( vtkRenderWindow, renWin );
    VTK_CREATE( vtkRenderWindowInteractor, iren );

    renWin->SetSize(500, 500);
    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);

    renderer->AddActor(actor);
    renderer->SetBackground(0.4392, 0.5020, 0.5647);

    VTK_CREATE(vtkContourFilter , contour);
    contour->SetInput( vtkImporter->GetOutput() );

    PixelType contourValue = 0.0;

    if( argc > 2 )
      {
      contourValue = atoi( argv[2] );
      }

    contour->SetValue( 0, contourValue );


    VTK_CREATE(vtkPolyDataMapper , polyMapper);
    VTK_CREATE(vtkActor          , polyActor );

    polyActor->SetMapper( polyMapper );
    polyMapper->SetInput( contour->GetOutput() );
    polyMapper->ScalarVisibilityOff();

    VTK_CREATE(vtkProperty , property);
    property->SetAmbient(0.1);
    property->SetDiffuse(0.1);
    property->SetSpecular(0.5);
    property->SetColor(1.0,0.0,0.0);
    property->SetLineWidth(2.0);
    property->SetRepresentationToSurface();

    polyActor->SetProperty( property );

    renderer->AddActor( polyActor );


    vtkCamera * cam = renderer->GetActiveCamera();
    cam->SetPosition(0, 1, 0);
    cam->SetFocalPoint(0, 0, 0);
    cam->SetViewUp(0, 0, -1);

    renderer->ResetCamera();
    renWin->Render();

    if (interactive)
      {
      iren->Start();
      }

    // Save screenshot if asked to.
    //
    for (int i = 1; i < argc-1; i++)
      {
      if (strcmp("-Screenshot", argv[i]) == 0)
        {
        VTK_CREATE( vtkWindowToImageFilter, windowToImageFilter );
        VTK_CREATE( vtkPNGWriter, screenShotWriter );

        windowToImageFilter->SetInput( renWin );
        windowToImageFilter->Update();

        std::string screenShotFileName = argv[i+1];

        std::cout << "Saving screenshot as " << screenShotFileName << std::endl;
        screenShotWriter->SetInput( windowToImageFilter->GetOutput() );
        screenShotWriter->SetFileName( screenShotFileName.c_str() );

        renWin->Render();
        screenShotWriter->Write();
        }
      }
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception catched !! " << e << std::endl;
    }

  return EXIT_SUCCESS;
}
