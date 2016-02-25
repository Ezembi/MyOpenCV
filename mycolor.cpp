#include "MyColor.h"

MyColor::MyColor()
{
    R = 0;
    G = 0;
    B = 0;
}

MyColor::MyColor(int Red, int Green, int Blue)
{
    R = Red;
    G = Green;
    B = Blue;
}

int MyColor::getRed(){
    return R;
}

int MyColor::getGreen(){
    return G;
}

int MyColor::getBlue(){
    return B;
}

void MyColor::setColor(int Red, int Green, int Blue){
    R = Red;
    G = Green;
    B = Blue;
}

void MyColor::setRed(int Red){
    R = Red;
}

void MyColor::setGreen(int Green){
    G = Green;
}

void MyColor::setBlue(int Blue){
    B = Blue;
}

int MyColor::getMonoColor(){
    R = G = B = (R * 0.299 + G * 0.587 + B * 0.114);
    return R;
}
