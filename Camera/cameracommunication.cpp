#include "cameracommunication.h"

CameraCommunication::CameraCommunication()
{

}

void CameraCommunication::Init()
{
    cam.Initialize();
    Setup_Wifi_and_Server(client);
    Send("{ Cam Con hello }", client);
    //initalize mulitplexer then select camera port, then initalize camera at port

}
void CameraCommunication::Update(uint8_t port){
    cam.tcaselect(port);
    cam.Read_Update();
}

CommunicationStatus CameraCommunication::ReconnectWifi()
{
    ConnectWifi();
}

CommunicationStatus CameraCommunication::ReconnectServer()
{
    ConnectServer(client);
}

CommunicationStatus CameraCommunication::CheckConnectionsStatus()
{
    if(!GetWifiConnected()) //need check wifi first b/c need wifi to connect to server
    {
        return ComWifiDisconnect;
    }
    if(!client)
    {
        return ComServerDisconnect;
    }
    return ComSucess;
}

CommunicationStatus CameraCommunication::HereIsPos()
{
    CommunicationStatus status=CheckConnectionsStatus();
    if(status==ComSucess){
        WaitForUpdate(client);
        Packet p=GetMessage(client);
        Serial.print("Message Got is: ");Serial.println(p.Message);
        if(strCmp(p.Command,COMM_REQ_POS)){
            String holdCoords=Convert();
            int length= holdCoords.length()+1;
            char coords[100];
            holdCoords.toCharArray(coords, length);
            //creates message with coords
            Cam_Coords(coords);
            Serial.print("coords ARE: ");Serial.println(coords);
            Send(coords,client);
            delay(300);
            return ComSucess;
        }
            else {
                Serial.print("Incorrect Message: "); Serial.println(p.Message);
                return ComError;
        }
    }
    return status;
}
String CameraCommunication:: Convert()
{
    Serial.println("Convert");
    int x=cam.Get_X_Coord();
    int y=cam.Get_Y_Coord();
    String hold;
    String hold_x(x );
    String hold_y(y );
    hold+=hold_x;
    hold +=" ";
    hold +=hold_y;
    Serial.print("hold: "); Serial.println(hold);
    return hold;
}
void CameraCommunication::Cam_Coords(char Sendme[])
{
    char message[100]="{ Cam Con HereIsPos ";
    append(message,Sendme);
    append(message," }");
    Sendme[0]='\0';
    append(Sendme,message);
}
