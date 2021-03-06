PROJECT(TrackingServerSimulator)

cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

find_package(OpenIGTLink REQUIRED)

include(${OpenIGTLink_USE_FILE})


set(TRACKING_SERVER_SOURCE
  OpenIGTLinkSockUtil.cxx
  ServerInfoBase.cxx
  ServerStateBase.cxx
  ServerInitializationState.cxx
  ServerStartUpState.cxx
  ServerStandByState.cxx
  ServerTrackingState.cxx
  SessionController.cxx
  TrackingServerSimulator.cxx
)


set(TEST_CLIENT_SOURCE
  OpenIGTLinkSockUtil.cxx
  TestClientProtocolBase.cxx
  TestClientProtocolNormal.cxx
  TestClientSimulator.cxx
)


ADD_EXECUTABLE(TrackingServerSimulator  ${TRACKING_SERVER_SOURCE})
TARGET_LINK_LIBRARIES(TrackingServerSimulator OpenIGTLink)

ADD_EXECUTABLE(TestClientSimulator  ${TEST_CLIENT_SOURCE})
TARGET_LINK_LIBRARIES(TestClientSimulator OpenIGTLink)

