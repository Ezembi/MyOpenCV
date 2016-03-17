#ifndef MYPICTURE_H
#define MYPICTURE_H
#include <defines.h>
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

    int ANMS(
            const double *Array_interesting_point, int point_count, double T
            );                              //Adaptive non-maximum suppression

public:
    int Width() const;                      //ширина изображения
    int Height() const;                     //высота изображения
    MyPicture(QString file);
    void ConvertToNegative();               //получить негатив изображения
    void ResizeImage(
            int NewWidth,
            int NewHeight);
    void AddNoise(int count_point);                     //добавление шума

    void HorizontalSwap();                  //отобразить по горизонтали
    void VerticalSwap();                    //отобразить по вертикали
    void Blur();                            //размытие
    void Sharpness();                       //резкость
    void Sobel(QString Param);              //собель

    /**
     * @brief Moravec - оператор Моравека
     * @param u - координаты в окне
     * @param v - координаты в окне
     * @param dx - сдвига окна по Х
     * @param dy - сдвига окна по У
     * @param px - окрестность локального максима Х
     * @param py - окрестность локального максима У
     * @param T - пороговое значение
     */
    void Moravec(int _u, int _v, int _dx, int _dy, int point_count, double T
            );

    void Harris();

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
