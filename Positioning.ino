//#define DEBUG

/*-----Wifi includes-----*/
//#include <SPI.h>
#include "Camera/camera.h"
#include "Camera/cameracommunication.h"
#include "CarProjCommon/CarProjCommon.h"
#include "CommunicationFuncs/CommunicationFuncs.h"
//int keyIndex = 0;                // your network key Index number (needed only for WEP)
//const int distanceInUnits = 50; //constant distance for converting to real world units function
////^^^USE MILLIMETERS WHENEVER POSSIBLE FOR ACCURATE CALIBRATION^^^
//int x1, x2;     //used for calculating distance between two points
//int y1, y2;     //used for calculating distance between two points
//double conversionRatio;
//String coordinates; //saves coordinates in a string to be sent as a message to the server/controller
//Camera Cam1;
CameraCommunication Cam_Comm;
CommunicationStatus com_status;
//
//void tcaselect(uint8_t i)
//{
//
//  if (i > 7) return;
//  Wire.beginTransmission(0x70);
//  Wire.write(1 << i);
//  Wire.endTransmission();
//}
//
//void Port(uint8_t port_number)
//{
//  tcaselect(port_number);
//  Cam1.Update();
//}

void setup() {

  Serial.begin(9600);
  Wire.begin();
//  tcaselect(0);
  Cam_Comm.Init();
//    Cam1.tcaselect(0);
  //first message to send to get the message loop gfoing. For testing/simulation
//  comm.client.print("{ Car Con LightIsOn }"); //no F() for stuff to client
  //com_status.client.print("{ Cam Con HereIsPos 0 1 }");
}

void loop() {
//  Port(0);
  Cam_Comm.Update(0);
  com_status=Cam_Comm.HereIsPos();
   if (com_status == ComSucess){
    //  info has been sent inside HereIsPos() function.
  }
  if (com_status == ComWifiDisconnect){
    // reconnect to wifi & server
    Cam_Comm.ReconnectWifi();
    Cam_Comm.ReconnectServer();
  }
  else if (com_status == ComServerDisconnect) {
    // reconnect to server
  Cam_Comm.ReconnectServer();
  }
  else{
    //unknown error
  }

  //  #ifdef DEBUG
  delay(3000);
  //  #endif
  //--------------------------

}
