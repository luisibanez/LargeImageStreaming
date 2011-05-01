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

// .NAME vtkInteractorStyleImageCursor - interactive manipulation of the camera specialized for images
// .SECTION Description
//
// Extends the vtkInteractorStyleImage to use cursor arrows to change image slices.
//
// Note that the renderer's actors are not moved; instead the camera is moved.

// .SECTION See Also
// vtkInteractorStyleImage

#ifndef __vtkInteractorStyleImageCursor_h
#define __vtkInteractorStyleImageCursor_h

#include "vtkInteractorStyleImage.h"
#include "vtkImageActor.h"
#include "vtkRenderWindow.h"


class VTK_RENDERING_EXPORT vtkInteractorStyleImageCursor : public vtkInteractorStyleImage
{
public:
  static vtkInteractorStyleImageCursor *New();
  vtkTypeMacro(vtkInteractorStyleImageCursor, vtkInteractorStyleImage);
  void PrintSelf(ostream& os, vtkIndent indent);

  void SetImageActor( vtkImageActor * );
  void SetRenderWindow( vtkRenderWindow * );

  // Description:
  // Override the key presses
  virtual void OnChar();

protected:
  vtkInteractorStyleImageCursor();
  ~vtkInteractorStyleImageCursor();

  virtual void GoToNextSlice();
  virtual void GoToPreviousSlice();
  virtual void RefreshRender();

private:
  vtkInteractorStyleImageCursor(const vtkInteractorStyleImageCursor&);  // Not implemented.
  void operator=(const vtkInteractorStyleImageCursor&);  // Not implemented.

  vtkImageActor   * ImageActor;
  vtkRenderWindow * RenderWindow;
};

#endif
