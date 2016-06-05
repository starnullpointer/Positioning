#ifndef CAMERACOMMUNICATION_H
#define CAMERACOMMUNICATION_H
//#include "../../CarProjCommon/CarProjCommon.h"
#include "../CommunicationFuncs/CommunicationFuncs.h"
#include "../Camera/camera.h"
#include <Adafruit_WINC1500Client.h>
//#include <SPI.h>
class CameraCommunication
{
public:
    CameraCommunication();
    void Init();
    void Update(uint8_t port);
    CommunicationStatus ReconnectWifi();
    CommunicationStatus ReconnectServer();
    CommunicationStatus HereIsPos();
    CommunicationStatus CheckConnectionsStatus();
    String Convert();
    void Cam_Coords(char Sendme[]);

private:
    Adafruit_WINC1500Client client;
    Camera cam;

};

#endif // CAMERACOMMUNICATION_H
