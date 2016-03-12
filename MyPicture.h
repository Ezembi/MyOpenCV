#ifndef MYPICTURE_H
#define MYPICTURE_H
#include <QImage>
#include <QRgb>
#include <QString>
#include <MyQImage.h>
#include <qmath.h>
#include <QtGlobal>

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

    void static printKernel(
            double *Kernel, int width, int height
            );

public:
    int Width() const;                      //ширина изображения
    int Height() const;                     //высота изображения
    MyPicture(QString file);
    void ConvertToNegative();               //получить негатив изображения
    void ResizeImage(
            int NewWidth,
            int NewHeight);

    void HorizontalSwap();                  //отобразить по горизонтали
    void VerticalSwap();                    //отобразить по вертикали
    void Blur();                            //размытие
    void Sharpness();                       //резкость
    void Sobel(QString Param);              //собель

    /**
     * @brief Pyramid                       //пирамидальное масштабирование
     * @param octave                        //кол-во октав (во скоько раз уменьшить изображение)
     * @param numLevel                      //кол-во уровней в октаве
     * @param finishOctaveLevel             //на каком уровне в октаве остановиться
     */
    void Pyramid(
            int octave, double numLevel, double finishOctaveLevel
            );

    void GaussianFilter(
            double r
            );                              //фильтр Гауса

    void GetGaussianKernel(
            double *Kernel, int width, int height, double r
            );                              //заполнения ядра Гауса

    void SavePictureInFile(QString file);   //сохранить изображение в файл
};

#endif // MYPICTURE_H
