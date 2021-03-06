/*=========================================================================

  Program:   OpenIGTLink Communication Server: Main
  Language:  C++

  Copyright (c) Brigham and Women's Hospital. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "ServerStateBase.h"
#include <string.h>
#include <stdlib.h>

#include "igtlOSUtil.h"
#include "igtlStringMessage.h"
#include "igtlClientSocket.h"
#include "igtlStatusMessage.h"
#include "igtlTransformMessage.h"
#include <cmath>


ServerStateBase::ServerStateBase()
{
  this->NextWorkphase.clear();
  this->ServerInfo = NULL;
}


ServerStateBase::~ServerStateBase()
{
}


//int ServerStateBase::Enter(const char* queryID)
int ServerStateBase::Enter()
{
  // Display the new phase
  std::cerr << "MESSAGE: Enter State: " << this->Name() << std::endl;
  return this->Initialize();
}


int ServerStateBase::Process()
{

  // Set the name of the current workphase as the next one.
  this->NextWorkphase = this->Name();

  // Create a message buffer to receive header
  igtl::MessageHeader::Pointer headerMsg;
  headerMsg = igtl::MessageHeader::New();

  // Recieve a message header
  this->SockUtil->ReceiveMessageHeader(headerMsg, 0);

  // Handle the message
  return this->MessageHandler(headerMsg); // Returns 0, if a workhpase change is requested.

}


int ServerStateBase::MessageHandler(igtl::MessageHeader* headerMsg)
{

  if (this->SockUtil->CheckMessageTypeAndName(headerMsg, "GET_STATUS", "STATE"))
    {
    this->SockUtil->SendStatusMessage("STATE", igtl::StatusMessage::STATUS_OK, 0, this->Name());
    return PHASE_CHANGE_NOT_REQUIRED;
    }
  else
    {
    return NOT_PROCESSED;
    }
}


//int ServerStateBase::CheckWorkphaseChange(igtl::MessageHeader* headerMsg)
//{
//
//  // Check if the message requests phase transition
//  if (strcmp(headerMsg->GetDeviceType(), "STRING") == 0 &&
//      strncmp(headerMsg->GetDeviceName(), "CMD_", 4) == 0)
//    {
//    igtl::StringMessage::Pointer stringMsg;
//    stringMsg = igtl::StringMessage::New();
//    stringMsg->SetMessageHeader(headerMsg);
//    stringMsg->AllocatePack();
//    int r = this->Socket->Receive(stringMsg->GetPackBodyPointer(), stringMsg->GetPackBodySize());
//    if (r < 0)
//      {
//      std::cerr << "ERROR: Timeout." << std::endl;
//      this->Socket->CloseSocket();
//      exit(EXIT_FAILURE);
//      }
//    else if (r == 0)
//      {
//      std::cerr << "ERROR: Socket closed while reading a message." << std::endl;
//      this->Socket->CloseSocket();
//      exit(EXIT_FAILURE);
//      }
//    
//    // Deserialize the string message
//    // If you want to skip CRC check, call Unpack() without argument.
//    int c = stringMsg->Unpack(1);
//    
//    if (c & igtl::MessageHeader::UNPACK_BODY) // if CRC check is OK
//      {
//      if (stringMsg->GetEncoding() == 3)
//        {
//        this->NextWorkphase = stringMsg->GetString();
//        // Get the query ID
//        std::string msgName = headerMsg->GetDeviceName();
//        this->QueryID = msgName.substr(4, std::string::npos);
//
//        return 1;
//        }
//      else
//        {
//        this->NextWorkphase = "Unknown";
//        return 1;
//        }
//      }
//    else
//      {
//      std::cerr << "ERROR: Invalid CRC." << std::endl;
//      this->NextWorkphase = "Unknown";
//      return 1;
//      }
//    }
//  else
//    {
//    return 0;
//    }
//}


int ServerStateBase::CheckCommonMessage(igtl::MessageHeader* headerMsg)
{
  /// Check if GET_TRANSFORM has been received
  if (strcmp(headerMsg->GetDeviceType(), "GET_TRANSFORM") == 0 &&
      strncmp(headerMsg->GetDeviceName(), "CURRENT_POSITION", 4) == 0)
    {
    return 1;
    }
  /// Check if GET_STATUS has been received
  else if (strcmp(headerMsg->GetDeviceType(), "GET_STATUS") == 0 &&
           strncmp(headerMsg->GetDeviceName(), "CURRENT_STATUS", 4) == 0)
    {
    this->SockUtil->SendStatusMessage(this->Name(), 1, 0);
    return 1;
    }

  return 0;
}



int ServerStateBase::SetDefectStatus(const char * type, int s)
{
  std::map< std::string, int >::iterator iter;
  iter = this->DefectStatus.find(type);
  if (iter != this->DefectStatus.end())
    {
    iter->second = ((s)?1:0);
    return 1;
    }
  else
    {
    // The specified type is not available
    return 0;
    }
}

int ServerStateBase::GetDefectStatus(const char * type)
{
  std::map< std::string, int >::iterator iter;
  iter = this->DefectStatus.find(type);
  if (iter != this->DefectStatus.end())
    {
    return iter->second;
    }
  else
    {
    return -1;
    }
}

std::list< std::string > ServerStateBase::GetDefectTypeList()
{
  std::list< std::string > list;
  list.clear();
  
  std::map< std::string, int >::iterator iter;
  for (iter = this->DefectStatus.begin(); iter != this->DefectStatus.end(); iter ++)
    {
    list.push_back(iter->first);
    }

  return list;
}

std::string ServerStateBase::GetDefectTypeDescription(const char * type)
{
  std::map< std::string, std::string >::iterator iter;
  iter = this->DefectDescription.find(type);
  if (iter != this->DefectDescription.end())
    {
    return iter->second;
    }
  else
    {
    return std::string("");
    }
}

int ServerStateBase::RegisterDefectType(const char* name, const char* desc)
{
  this->DefectStatus[name] = 0; // set 0 status
  this->DefectDescription[name] = desc;
  return 1;
}
