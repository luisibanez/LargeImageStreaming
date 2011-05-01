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

#include "vtkInteractorStyleImageCursor.h"

#include "vtkCallbackCommand.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkInteractorStyleImageCursor);

//----------------------------------------------------------------------------
vtkInteractorStyleImageCursor::vtkInteractorStyleImageCursor()
{
  this->ImageActor = NULL;
  this->RenderWindow = NULL;
}

//----------------------------------------------------------------------------
vtkInteractorStyleImageCursor::~vtkInteractorStyleImageCursor()
{
}

//----------------------------------------------------------------------------
void vtkInteractorStyleImageCursor::SetImageActor( vtkImageActor * actor )
{
  this->ImageActor = actor;
}

//----------------------------------------------------------------------------
void vtkInteractorStyleImageCursor::SetRenderWindow( vtkRenderWindow * renderWindow )
{
  this->RenderWindow = renderWindow;
}

//----------------------------------------------------------------------------
void vtkInteractorStyleImageCursor::RefreshRender()
{
  if( this->RenderWindow )
    {
    this->RenderWindow->Render();
    }
}

//----------------------------------------------------------------------------
void vtkInteractorStyleImageCursor::GoToNextSlice()
{
  if( this->ImageActor )
    {
    int currentSlice = this->ImageActor->GetSliceNumber();
    int nextSlice = currentSlice + 1;
    if( nextSlice < this->ImageActor->GetSliceNumberMax() )
      {
      this->ImageActor->SetZSlice( nextSlice );
      }
    std::cout << "Slice : " << nextSlice << std::endl;
    }
  this->RefreshRender();
}

//----------------------------------------------------------------------------
void vtkInteractorStyleImageCursor::GoToPreviousSlice()
{
  if( this->ImageActor )
    {
    int currentSlice = this->ImageActor->GetSliceNumber();
    int previousSlice = currentSlice - 1;
    if( previousSlice > this->ImageActor->GetSliceNumberMin() )
      {
      this->ImageActor->SetZSlice( previousSlice );
      }
    std::cout << "Slice : " << previousSlice << std::endl;
    }
  this->RefreshRender();
}

//----------------------------------------------------------------------------
void vtkInteractorStyleImageCursor::OnChar()
{
  vtkRenderWindowInteractor *rwi = this->Interactor;

  switch (rwi->GetKeyCode())
    {
    case '.' :
    case '>' :
      {
      this->GoToNextSlice();
      break;
      }
    case ',' :
    case '<' :
      {
      this->GoToPreviousSlice();
      break;
      }
    default:
      this->Superclass::OnChar();
      break;
    }
}

//----------------------------------------------------------------------------
void vtkInteractorStyleImageCursor::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
