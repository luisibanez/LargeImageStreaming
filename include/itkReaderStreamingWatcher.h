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
#ifndef _itkReaderStreamingWatcher_h
#define _itkReaderStreamingWatcher_h

#include "itkCommand.h"
#include "itkTimeProbesCollectorBase.h"

namespace itk {

class ReaderStreamingWatcher
{
public:
  ReaderStreamingWatcher(itk::ProcessObject* o)
  {
    m_Process = o;
    m_Steps = 0;

    itk::SimpleMemberCommand<ReaderStreamingWatcher>::Pointer startFilterCommand;
    itk::SimpleMemberCommand<ReaderStreamingWatcher>::Pointer endFilterCommand;
    itk::SimpleMemberCommand<ReaderStreamingWatcher>::Pointer progressFilterCommand;

    startFilterCommand =    itk::SimpleMemberCommand<ReaderStreamingWatcher>::New();
    endFilterCommand =      itk::SimpleMemberCommand<ReaderStreamingWatcher>::New();
    progressFilterCommand = itk::SimpleMemberCommand<ReaderStreamingWatcher>::New();

    startFilterCommand->SetCallbackFunction(this, &ReaderStreamingWatcher::StartFilter);
    endFilterCommand->SetCallbackFunction(this, &ReaderStreamingWatcher::EndFilter);
    progressFilterCommand->SetCallbackFunction(this, &ReaderStreamingWatcher::ShowProgress);

    m_Process->AddObserver(itk::StartEvent(), startFilterCommand);
    m_Process->AddObserver(itk::EndEvent(), endFilterCommand);
    m_Process->AddObserver(itk::ProgressEvent(), progressFilterCommand);
  }

  virtual ~ReaderStreamingWatcher() {}

  virtual void ShowProgress()
  {
    m_Steps++;

    std::cout << " | " << m_Process->GetProgress() << std::flush;

    if ( (m_Steps % 10) == 0 )
      {
      std::cout << std::endl;
      }
  }

  virtual void StartFilter()
  {
    this->m_Chronometer.Start("Reading");

    m_Steps = 0;

    std::cout << "-------- Start " << m_Process->GetNameOfClass();

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
    this->m_Chronometer.Stop("Reading");
    this->m_Chronometer.Report( std::cout );
  }

protected:
  int             m_Steps;

  itk::TimeProbesCollectorBase m_Chronometer;

  itk::ProcessObject::Pointer m_Process;

private:
  ReaderStreamingWatcher(); // Purposely not implemented
};

}

#endif
