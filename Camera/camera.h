#ifndef CAMERA_H
#define CAMERA_H
#include <Arduino.h>
#include <Wire.h>
#include "../CarProjCommon/CarProjCommon.h"
#include <SPI.h>

class Camera
{
public:
    Camera();
    void Read_Update();
    void Initialize();
    void Write_2bytes(byte d1, byte d2);
    void tcaselect(uint8_t i);

    int Get_X_Coord();
    int Get_Y_Coord();
    
    void Calibration(uint8_t port_number);
    int Distance(int x1, int x2, int y1, int y2);
    int ConvertToUnits(int convertThis);
    
    int slaveAddress= 0x58;
    byte data_buf[16];          //data from camera
    int Ix[4]; //int array for x coordinates
    int Iy[4]; //int array for y coordinates
    int temp_x;// to store coordinates
    int temp_y;//to store coordinates
    char x[10];
    char y[10];
    double conversionRatio;
};

#endif // CAMERA_H
