#include "camera.h"

Camera::Camera()
{

}

void Camera::Read_Update()
{
    int s;
    Wire.beginTransmission(slaveAddress);
    Wire.write(0x36);
    Wire.endTransmission();
    Wire.requestFrom(slaveAddress, 16);        // Request the 2 byte heading (MSB comes first)
    for (int i = 0; i < 16; i++) {
      data_buf[i] = 0;
    }
    int i = 0;
    while (Wire.available() && i < 16) {
      data_buf[i] = Wire.read();
      i++;
    }
    Ix[0] = data_buf[1];
    Iy[0] = data_buf[2];
    s   = data_buf[3];
    Ix[0] += (s & 0x30) << 4;
    Iy[0] += (s & 0xC0) << 2;

    Ix[1] = data_buf[4];
    Iy[1] = data_buf[5];
    s   = data_buf[6];
    Ix[1] += (s & 0x30) << 4;
    Iy[1] += (s & 0xC0) << 2;

    Ix[2] = data_buf[7];
    Iy[2] = data_buf[8];
    s   = data_buf[9];
    Ix[2] += (s & 0x30) << 4;
    Iy[2] += (s & 0xC0) << 2;

    Ix[3] = data_buf[10];
    Iy[3] = data_buf[11];
    s   = data_buf[12];
    Ix[3] += (s & 0x30) << 4;
    Iy[3] += (s & 0xC0) << 2;
  for (int i=0; i<4; i++){
//    Serial.print(Ix[i]); Serial.print(' '); Serial.println(Iy[i]);
    Serial.print(Ix[i]); Serial.print(' '); Serial.println(Iy[i]);
  }
    temp_x=Ix[0];
    temp_y=Iy[0];
    Serial.print("Ix[0] "); Serial.print(temp_x); Serial.print(' ');Serial.print("Iy[0] "); Serial.println(temp_y);
}
int Camera::Get_X_Coord(){
    int num=0;
     if(Ix[num]==1023){
         num++;
     }
    temp_x=Ix[0];
    if(num==4){
        num=0;
    }
    return temp_x;
}
int Camera::Get_Y_Coord(){
    int num=0;
    if(Iy[num]==1023){
        num++;
    }
    temp_y=Iy[0];
    if(num==4){
        num=0;
    }
    return temp_y;
}
void Camera::Initialize()
{

      // IR sensor initialize
    Write_2bytes(0x30, 0x01); delay(10);
    Write_2bytes(0x30, 0x08); delay(10);
    Write_2bytes(0x06, 0x90); delay(10);
    Write_2bytes(0x08, 0xC0); delay(10);
    Write_2bytes(0x1A, 0x40); delay(10);
    Write_2bytes(0x33, 0x33); delay(10);
    delay(1000);
}
void Camera::Write_2bytes(byte d1, byte d2)
{
  Wire.beginTransmission(slaveAddress);
  Wire.write(d1);
  Wire.write(d2);
  Wire.endTransmission();
}

void Camera::tcaselect(uint8_t i)
{

  if (i > 7) return;
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  Wire.endTransmission();
}

//-----Calibration-----
void Camera::Calibration(uint8_t port_number){ //Calibration for when both leds are on at the same time
    double distance;
    int x1;
    int y1;
    int x2;
    int y2;
    do{
//        tcaselect(port_number);
        Wire.beginTransmission(slaveAddress);
        Wire.write(0x36);
        Wire.endTransmission();
        Wire.requestFrom(slaveAddress, 16);        // Request the 2 byte heading (MSB comes first)
        for (int i = 0; i < 16; i++) {
            data_buf[i] = 0;
        }
        int i = 0;
        while (Wire.available() && i < 16) {
            data_buf[i] = Wire.read();
            i++;
        }
        
        Ix[0] = data_buf[1];
        Iy[0] = data_buf[2];
        int s   = data_buf[3];
        Ix[0] += (s & 0x30) << 4;
        Iy[0] += (s & 0xC0) << 2;
        
        x1 = Ix[0];
        y1 = Iy[0];
        
//#ifdef DEBUG
//        Serial.println(Ix[0]);          //For debugging
//        Serial.println(Iy[0]);          //with the debugging in we don't have enough space to run wifi/messaging at the same time
//        Serial.println(x1);
//        Serial.println(y1);
//#endif
        
        Ix[1] = data_buf[4];
        Iy[1] = data_buf[5];
        s   = data_buf[6];
        Ix[1] += (s & 0x30) << 4;
        Iy[1] += (s & 0xC0) << 2;
        
        x2 = Ix[1];
        y2 = Iy[1];
        
//#ifdef DEBUG
//        Serial.println(Ix[1]);          //For debugging
//        Serial.println(Iy[1]);
//        Serial.println(x2);
//        Serial.println(y2);
//        Serial.println(F(""));
//        delay(2000);
//#endif
        
    }while(x1 == 1023 || y1 == 1023 || x2 == 1023 || y2 == 1023);
    
    distance = Distance(x1, x2, y1, y2);
    
    int distanceInUnits= ConvertToUnits(distance);
    
    conversionRatio = distanceInUnits / distance;
//
//#ifdef DEBUG
//    Serial.println(distance);         //For debugging
//    Serial.println(conversionRatio);  //For debugging
//#endif
}

//-----Calculates Distance Between Two Points-----
int Camera::Distance(int x1, int x2, int y1, int y2){   //USE MILLIMETERS
    return sqrt(pow((double)x2-x1,2)+pow((double)y2-y1,2));
}

//-----Converting Pixels to Real World Units-----
int Camera::ConvertToUnits(int convertThis ){
    return conversionRatio * convertThis;
}
