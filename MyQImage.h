#ifndef MYQIMAGE_H
#define MYQIMAGE_H
#include <QImage>
#include <QRgb>
#include <QString>

class MyQImage
{
private:

    double *Image;                                  //изображение в моноцвете
    int height;                                     //высота
    int width;                                      //ширина
    bool load;                                      //статус загрузки изображения
    QImage::Format format;                          //формат изображения, для сохранения

private:

    void setWidth(int _width);                      //получить ширину изображения
    void setHeight(int _height);                    //получить высоту изображения
    void SetLoad(bool Load);                        //загрузка изображенияы

public:
    void LoadImage(QString file);                   //загрузка изображения

    MyQImage(QString file);
    MyQImage();

    double checkColor(double color);                //проверка значения цвета на 0...255
    double getMonoColor(QRgb color);                //получить моно цвет пикселя
    int Width();                                    //получить ширину изображения
    int Height();                                   //получить высоту изображения
    bool IsLoad();                                  //проверна на загруженность изображения
    void SetPixel(int x, int y, double color);      //задаёт цвет пикселя
    double GetPixel(int x, int y);                  //получить моно цвет
    QRgb GetColorPixel(int x, int y);               //получить цветной пиксель
    void SwapPixel(int x1, int y1, int x2, int y2); //поменять пиксели местами
    void HorizontalSwap();                          //отобразить по горизонтали
    void WerticalSwap();                            //отобразить по вертикали
    void SaveImage(QString file);                   //сохранение изображение
};

#endif // MYQIMAGE_H
