#ifndef MYPICTURE_H
#define MYPICTURE_H
#include <QImage>
#include <QRgb>
#include <QString>
#include <MyQImage.h>
#include <qmath.h>

/**
 * @brief мой класс обработки изображения
 */

class MyPicture
{
private:

    MyQImage Result;                        //обработанное изображение

private:
    double static GetSobelGradient(
            double Gx, double Gy
            );                              //получить значение градиента

public:
    int Width() const;                      //ширина изображения
    int Height() const;                     //высота изображения
    MyPicture(QString file);
    void ConvertToNegative();               //получить негатив изображения

    void HorizontalSwap();                  //отобразить по горизонтали
    void VerticalSwap();                    //отобразить по вертикали
    void Blur();                            //размытие
    void Sharpness();                       //резкость
    void Sobel(QString Param);              //собель
/*
    void GaussianFilter(double _r
            );                              //фильтр Гауса
    double GaussianConvolution(int _x, int _y, double _r
            );
*/
    void SavePictureInFile(QString file);   //сохранить изображение в файл
};

#endif // MYPICTURE_H
