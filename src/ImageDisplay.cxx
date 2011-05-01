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
    std::cerr << "Usage: " << argv[0] << " inputImageFilename " << std::endl;
    return EXIT_FAILURE;
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

    vtkSmartPointer< vtkImageImport > vtkImporter =
      vtkSmartPointer< vtkImageImport >::New();

    ConnectPipelines(itkExporter, vtkImporter);

    vtkImporter->UpdateInformation();

    int * extent = vtkImporter->GetOutput()->GetWholeExtent();

    int slice_min = extent[2];
    int slice_max = extent[2 + 1];

    //------------------------------------------------------------------------
    // VTK visualization pipeline
    //------------------------------------------------------------------------

    vtkSmartPointer< vtkImageActor > actor =
      vtkSmartPointer< vtkImageActor >::New();

    vtkSmartPointer< vtkInteractorStyleImageCursor > interactorStyle =
      vtkSmartPointer< vtkInteractorStyleImageCursor >::New();

    vtkSmartPointer< vtkRenderer > renderer =
      vtkSmartPointer< vtkRenderer >::New();

    vtkSmartPointer< vtkRenderWindow > renWin =
      vtkSmartPointer< vtkRenderWindow >::New();

    vtkSmartPointer< vtkRenderWindowInteractor > iren =
      vtkSmartPointer< vtkRenderWindowInteractor >::New();

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
