#ifndef MYPIXEL_H
#define MYPIXEL_H
#include <MyColor.h>


class MyPixel {

private:

    MyColor color;

    int _X;
    int _Y;

public:
    MyPixel();
    MyPixel(int X, int Y, MyColor Color);

    MyColor getColor();
    int getX();
    int getY();

    void setColor(MyColor Color);
    void setX(int X);
    void setY(int Y);

};

#endif // MYPIXEL_H
