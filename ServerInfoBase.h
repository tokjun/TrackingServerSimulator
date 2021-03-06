/*=========================================================================

  Program:   OpenIGTLink Communication Server: Status
  Language:  C++

  Copyright (c) Brigham and Women's Hospital. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __ServerInfoBase_h
#define __ServerInfoBase_h

#include <string>
#include <map>

#include "igtlSocket.h"
#include "igtlMath.h"
#include "igtlMessageBase.h"
#include "igtlMutexLock.h"

class ServerInfoBase
{
public:
  ServerInfoBase();
  ~ServerInfoBase();

  void SetCalibrationMatrix(igtl::Matrix4x4& matrix);

  // Return 0 if a calibration matrix has not been set.
  int GetCalibrationMatrix(igtl::Matrix4x4& matrix);
  void SetTargetMatrix(igtl::Matrix4x4& matrix);

  // Return 0 if a target matrix has not been set.
  int GetTargetMatrix(igtl::Matrix4x4& matrix);

  // Temporal resolution for tracking
  void SetTrackingResolution(int res) { this->TrackingResolution = res; }
  int GetTrackingResolution() { return this->TrackingResolution; }
  
  // Name of the tracking coordinate system
  void SetTrackingCoordinate(const char* str) { this->TrackingCoordinate = str; }
  std::string GetTrackingCoordinate() { return this->TrackingCoordinate; }


protected:

  int TrackingResolution;
  std::string TrackingCoordinate;

  int FlagCalibration;
  int FlagTarget;

  igtl::Matrix4x4 CalibrationMatrix;
  igtl::Matrix4x4 TargetMatrix;
};



#endif //__ServerInfoBase_h
