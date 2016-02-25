#ifndef MYPICTURE_H
#define MYPICTURE_H
#include <QImage>
#include <QRgb>
#include <QString>
#include <MyQImage.h>

/**
 * @brief мой класс изображения
 */

class MyPicture
{
private:
    bool load;                              //статус загрузки изобрадения

    MyQImage Result;                        //обработанное изображение

private:
    void SetLoad(bool _load);               //установить статус загрузки изобрадения
    int getN(int x0, int x1,int y0,int y1); //посчтитать кол-во элементов в ядре свёртки
    double * Clone(MyQImage image);         //дублировать изображение
    double GetSobelGradient(
            double Gx, double Gy
            );                              //получить значение градиента

public:
    int Width();                            //ширина изображения
    int Height();                           //высота изображения
    bool IsLoad();                          //проверка на загруженность изображения
    MyPicture(QString file);
    void ConvertToNegative();               //получить негатив изображения

    void HorizontalSwap();                  //отобразить по горизонтали
    void WerticalSwap();                    //отобразить по вертикали
    void Blur();                            //размытие
    void Sharpness();                       //резкость
    void Sobel(QString Param);              //собель

    double Convolution(
            int *Kernel, int u, int v, int x, int y
            );                              //свёртка
    double Convolution(
            double * image,int *Kernel, int u, int v, int x, int y
            );                              //свёртка для собеля, что бы не использовать изменённые пиксели

    void SavePictureInFile(QString file);   //сохранить изображение в файл
};

#endif // MYPICTURE_H
