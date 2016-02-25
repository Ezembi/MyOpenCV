#include "MyPixel.h"

MyPixel::MyPixel() {
    color = MyColor(0,0,0);
    _X = 0;
    _Y = 0;
}

MyPixel::MyPixel(int X, int Y, MyColor Color){
    color = Color;
    _X = X;
    _Y = Y;
}

MyColor MyPixel::getColor(){
    return color;

}

int MyPixel::getX(){
    return _X;
}

int MyPixel::getY(){
    return _Y;
}

void MyPixel::setColor(MyColor Color){
    color = Color;
}

void MyPixel::setX(int X){
    _X = X;
}

void MyPixel::setY(int Y){
    _Y = Y;
}
