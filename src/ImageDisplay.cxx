/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkReadITKImageShowVTK.cxx,v $
  Language:  C++
  Date:      $Date: 2006-06-16 18:29:01 $
  Version:   $Revision: 1.5 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkImage.h"
#include "itkVTKImageExport.h"
#include "itkImageFileReader.h"

#include "vtkImageData.h"
#include "vtkImageImport.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"

#include "itkReaderStreamingWatcher.h"

template <typename ITK_Exporter, typename VTK_Importer>
void ConnectPipelines(ITK_Exporter exporter, VTK_Importer* importer)
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

    vtkImageImport* vtkImporter = vtkImageImport::New();
    ConnectPipelines(itkExporter, vtkImporter);


    //------------------------------------------------------------------------
    // VTK visualization pipeline
    //------------------------------------------------------------------------

    // Create a vtkImageActor to help render the image.  Connect it to
    // the vtkImporter instance.
    vtkImageActor* actor = vtkImageActor::New();
    actor->SetInput(vtkImporter->GetOutput());

    vtkInteractorStyleImage * interactorStyle = vtkInteractorStyleImage::New();

    // Create a renderer, render window, and render window interactor to
    // display the results.
    vtkRenderer* renderer = vtkRenderer::New();
    vtkRenderWindow* renWin = vtkRenderWindow::New();
    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();

    renWin->SetSize(500, 500);
    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);
    iren->SetInteractorStyle( interactorStyle );

    // Add the vtkImageActor to the renderer for display.
    renderer->AddActor(actor);
    renderer->SetBackground(0.4392, 0.5020, 0.5647);

    // Bring up the render window and begin interaction.
    renWin->Render();
    iren->Start();

    // Release all VTK components
    actor->Delete();
    interactorStyle->Delete();
    vtkImporter->Delete();
    renWin->Delete();
    renderer->Delete();
    iren->Delete();

    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception catched !! " << e << std::endl;
    }

  return EXIT_SUCCESS;
}
