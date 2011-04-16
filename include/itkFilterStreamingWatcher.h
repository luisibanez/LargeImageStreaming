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
#ifndef _itkFilterStreamingWatcher_h
#define _itkFilterStreamingWatcher_h

#include "itkCommand.h"
#include <time.h>

namespace itk {

class FilterStreamingWatcher
{
public:
  FilterStreamingWatcher(itk::ProcessObject* o, const char *comment="")
  {
    m_Start = 0;
    m_End = 0;
    m_Process = o;
    m_Steps = 0;
    m_Comment = comment;

    itk::SimpleMemberCommand<FilterStreamingWatcher>::Pointer startFilterCommand;
    itk::SimpleMemberCommand<FilterStreamingWatcher>::Pointer endFilterCommand;
    itk::SimpleMemberCommand<FilterStreamingWatcher>::Pointer progressFilterCommand;
    itk::SimpleMemberCommand<FilterStreamingWatcher>::Pointer iterationFilterCommand;
    itk::SimpleMemberCommand<FilterStreamingWatcher>::Pointer abortFilterCommand;

    startFilterCommand =    itk::SimpleMemberCommand<FilterStreamingWatcher>::New();
    endFilterCommand =      itk::SimpleMemberCommand<FilterStreamingWatcher>::New();
    progressFilterCommand = itk::SimpleMemberCommand<FilterStreamingWatcher>::New();
    iterationFilterCommand = itk::SimpleMemberCommand<FilterStreamingWatcher>::New();
    abortFilterCommand = itk::SimpleMemberCommand<FilterStreamingWatcher>::New();

    startFilterCommand->SetCallbackFunction(this, &FilterStreamingWatcher::StartFilter);
    endFilterCommand->SetCallbackFunction(this, &FilterStreamingWatcher::EndFilter);
    progressFilterCommand->SetCallbackFunction(this, &FilterStreamingWatcher::ShowProgress);
    iterationFilterCommand->SetCallbackFunction(this, &FilterStreamingWatcher::ShowIteration);

    m_Process->AddObserver(itk::StartEvent(), startFilterCommand);
    m_Process->AddObserver(itk::EndEvent(), endFilterCommand);
    m_Process->AddObserver(itk::ProgressEvent(), progressFilterCommand);
    m_Process->AddObserver(itk::IterationEvent(), iterationFilterCommand);
    m_Process->AddObserver(itk::AbortEvent(), abortFilterCommand);
  }

  virtual ~FilterStreamingWatcher() {}

  virtual void ShowProgress()
  {
    m_Steps++;

    std::cout << " | " << m_Process->GetProgress() << std::flush;

    if ( (m_Steps % 10) == 0 )
      {
      std::cout << std::endl;
      }
  }

  virtual void ShowIteration()
  {
    std::cout << " # " << std::flush;
    m_Iterations++;
  }

  virtual void StartFilter()
  {
    m_Steps = 0;
    m_Iterations = 0;
    m_Start = ::clock();

    std::cout << "-------- Start " << m_Process->GetNameOfClass()
              << " \"" << m_Comment << std::endl;

    itk::ProcessObject::DataObjectPointerArray outputsArray = m_Process->GetOutputs();

    if( ! outputsArray.empty() )
      {
      itk::DataObject::Pointer dataObject = outputsArray[0];

      typedef ImageBase<3> ImageType;

      const ImageType * image = dynamic_cast< const ImageType * >( dataObject.GetPointer() );

      if( image )
        {
        std::cout << image->GetRequestedRegion() << std::endl;
        }
      }

     std::cout << std::flush << std::endl;
    }

  const char *GetNameOfClass () {return m_Process->GetNameOfClass();}

  virtual void EndFilter()
  {
    m_End = ::clock();
    std::cout << std::endl << "Filter took "
              << static_cast<double>(m_End - m_Start) / CLOCKS_PER_SEC
              << " seconds.";
    std::cout << std::endl << std::endl
              << "-------- End " << m_Process->GetNameOfClass()
              << " \"" << m_Comment << "\" "
              << std::flush
              << std::endl;

    if (m_Steps < 1)
      {
      itkExceptionMacro ("Filter does not have progress.");
      }

    }

protected:
  clock_t         m_Start;
  clock_t         m_End;

  int             m_Steps;
  int             m_Iterations;
  std::string     m_Comment;

  itk::ProcessObject::Pointer m_Process;

private:
  FilterStreamingWatcher(); // Purposely not implemented
};

}

#endif
