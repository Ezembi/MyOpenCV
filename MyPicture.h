#ifndef MYPICTURE_H
#define MYPICTURE_H
#include <QImage>
#include <QRgb>
#include <QString>
#include <MyQImage.h>
#include <math.h>

/**
 * @brief мой класс обработки изображения
 */

class MyPicture
{
private:

    MyQImage Result;                        //обработанное изображение

private:
    double GetSobelGradient(
            double Gx, double Gy
            );                              //получить значение градиента

public:
    int Width() const;                      //ширина изображения
    int Height() const;                     //высота изображения
    MyPicture(QString file);
    void ConvertToNegative();               //получить негатив изображения

    int reflect(int x,int MaxX);            //обработка краевых эффектов отражением

    void HorizontalSwap();                  //отобразить по горизонтали
    void WerticalSwap();                    //отобразить по вертикали
    void Blur();                            //размытие
    void Sharpness();                       //резкость
    void Sobel(QString Param);              //собель

    double Convolution(
            const int *Kernel, int u, int v, int x, int y
            );                              //свёртка
    double Convolution(const MyQImage& image, const int *Kernel, int u, int v, int x, int y
            );                              //свёртка для собеля, что бы не использовать изменённые пиксели

    void SavePictureInFile(QString file);   //сохранить изображение в файл
};

#endif // MYPICTURE_H
