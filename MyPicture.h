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
    bool load;

    MyQImage Original;                      //оригинальное изображение

    MyQImage Result;                        //обработанное изображение

private:
    void SetLoad(bool _load);

    int getMonoColor(QRgb color);           //получить моно цвет пикселя
    QRgb getMonoPixel(QRgb color);          //получить пиксель с моноцветом
    QRgb getCustomPixel(int Red, int Green, int Blue);  //получить пиксель кастомного цвета

    int getN(int x0, int x1,int y0,int y1); //посчтитать кол-во элементов в ядре свёртки

    QRgb GetSobelGradient(QRgb Gx, QRgb Gy);//получить значение градиента

public:
    int Width();                            //ширина изображения
    int Height();                           //высота изображения

    bool IsLoad();                          //проверка на загруженность изображения

    MyPicture(QString file);
    void ResetToOriginal();                 //сброс исзображения до начального состояния

    double checkColor(double color);

    void ConvertToMonochrome();             //перевести изображение в чёрно-белое
    void ConvertToNegative();               //получить негатив изображения
    void HorizontalSwap();                  //отобразить по горизонтали
    void WerticalSwap();                    //отобразить по вертикали
    void Blur();                            //размытие
    void Sharpness();                       //резкость
    void Sobel(QString Param);              //собель

    /**
     * @brief Convolution   свёртка с моноцветом
     * @param Kernel        ядро свёртки
     * @param u             ширина свёртки
     * @param v             высота свёртки
     * @param x             координата точки
     * @param y             координата точки
     */
    double Convolution(int *Kernel, int u, int v, int x, int y);

    /**
     * @brief Convolution   свёртка
     * @param Kernel        ядро свёртки
     * @param u             ширина свёртки
     * @param v             высота свёртки
     * @param x             координата точки
     * @param y             координата точки
     */
    double Convolution(MyQImage image,int *Kernel, int u, int v, int x, int y);

    /**
     * @brief Convolution   свёртка
     * @param Kernel        ядро свёртки
     * @param u             ширина свёртки
     * @param v             высота свёртки
     * @param x             координата точки
     * @param y             координата точки
     */
    QRgb ConvolutionColor(int *Kernel, int u, int v, int x, int y);

    /**
     * @brief Convolution   свёртка
     * @param Kernel        ядро свёртки
     * @param u             ширина свёртки
     * @param v             высота свёртки
     * @param x             координата точки
     * @param y             координата точки
     */
    QRgb ConvolutionColor(MyQImage image,int *Kernel, int u, int v, int x, int y);

    void SavePictureInFile(QString file);           //сохранить изображение в файл
};

#endif // MYPICTURE_H
