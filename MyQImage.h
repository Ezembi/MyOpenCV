#ifndef MYQIMAGE_H
#define MYQIMAGE_H
#include <defines.h>
#include <time.h>
#include <QImage>
#include <QRgb>
#include <QString>
#include <memory>
#include <algorithm>

class MyQImage
{
private:

    std::unique_ptr<double[]> Image;
    int height;                                         //высота
    int width;                                          //ширина
    QImage::Format format;                              //формат изображения, для сохранения

    int count_interesting_point;

    double max;
    double min;

    double minOrigial;
    double maxOrigial;

private:

    void setWidth(int _width);                          //получить ширину изображения
    void setHeight(int _height);                        //получить высоту изображения

public:
    void LoadImage(QString file);                       //загрузка изображения

    MyQImage(QString file);
    MyQImage();
    MyQImage(const MyQImage& data);                     //конструктор копирования
    ~MyQImage();

    void getNormalStep();                               //получить коэффициент нормализации
    int getNormalNumber(double Number) const;           //получить нормализованное число
    double getMonoColor(QRgb color);                    //получить моно цвет пикселя

    int Width() const;                                  //получить ширину изображения
    int Height() const;                                 //получить высоту изображения
    void SetPixel(int x, int y, double color);          //задаёт цвет пикселя
    double GetPixel(int x, int y) const;                //получить моно цвет
    QRgb GetColorPixel(int x, int y);                   //получить цветной пиксель (нормализация)
    void SwapPixel(int x1, int y1, int x2, int y2);     //поменять пиксели местами
    void HorizontalSwap();                              //отобразить по горизонтали
    void VerticalSwap();                                //отобразить по вертикали
    void AddNoise(int count_point);                     //добавление шума
    void ResizeImage(
            int NewHeight,
            int NewWidth
            );                                          //изменить размер изображения

    void Convolution(
            const double *Kernel, int u, int v
            );                                          //свёртка
    void Convolution(
            const MyQImage& image, const double *Kernel, int u, int v
            );                                          //свёртка для собеля, что бы не использовать изменённые пиксели

    void SaveImage(QString file);                       //сохранение изображение
};

#endif // MYQIMAGE_H
