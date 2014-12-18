/*=========================================================================

  Program:   OpenIGTLink Communication Server: StartUp Phase
  Language:  C++

  Copyright (c) Brigham and Women's Hospital. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "ServerStartUpPhase.h"
#include <string.h>
#include <stdlib.h>

#include "igtlOSUtil.h"
#include "igtlStringMessage.h"
#include "igtlClientSocket.h"
#include "igtlStatusMessage.h"
#include "igtlTransformMessage.h"
#include <cmath>

ServerStartUpPhase::ServerStartUpPhase() :
  ServerPhaseBase()
{
  // Register Device-not-ready defect
  this->RegisterDefectType("DNR", "Device-not-ready in START_UP phase.");
}


ServerStartUpPhase::~ServerStartUpPhase()
{
}

int ServerStartUpPhase::Initialize()
{

  this->SendStatusMessage("STATE", igtl::StatusMessage::STATUS_OK, 0, this->Name());

  // Send Status after waiting for 2 seconds (mimicking initialization process)
  igtl::Sleep(2000); // wait for 2000 msec

  if (this->GetDefectStatus("DNR"))
    {
    // Device-not-ready defect is active
    this->SendStatusMessage(this->Name(), igtl::StatusMessage::STATUS_NOT_READY, 0);
    }
  else
    {
    // Normal
    this->SendStatusMessage(this->Name(), 1, 0);
    }

  return 1;
}


int ServerStartUpPhase::MessageHandler(igtl::MessageHeader* headerMsg)
{

  if (ServerPhaseBase::MessageHandler(headerMsg))
    {
    return 1;
    }

  return 0;
}


int ServerStartUpPhase::TimerHandler(long timestamp)
{

  std::cerr << "ServerStartUpPhase::TimerHandler() is called" << std::endl;
  return 0;

}
