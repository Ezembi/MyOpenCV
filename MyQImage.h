#ifndef MYQIMAGE_H
#define MYQIMAGE_H
#include <GlobalNames.h>
#include <time.h>
#include <QImage>
#include <QRgb>
#include <QString>
#include <memory>
#include <algorithm>
#include <assistant.h>

class MyQImage
{
private:

    std::unique_ptr<double[]> Image_;
    int height_;                                         //высота
    int width_;                                          //ширина
    QImage::Format format_ = QImage::Format_RGB32;       //формат изображения, для сохранения

    int nInterestingPoint_;

    double min_ = 0;
    double max_ = 1;

    double minOrigial_ = 0;
    double maxOrigial_ = 256;

private:

    void setWidth(int width);                           //получить ширину изображения
    void setHeight(int height);                         //получить высоту изображения

public:
    void loadImage(QString file);                       //загрузка изображения

    MyQImage(QString file);
    MyQImage();
    MyQImage(int width, int height);                    //создать пустое изображение
    MyQImage(
            const MyQImage& data,
            int newWidth,
            int newHeight
            );                                          //создать изображение другого масштаба

    MyQImage(const MyQImage& data);                     //конструктор копирования
    ~MyQImage();

    int getNormalNumber(double number) const;           //получить нормализованное число
    double getMonoColor(QRgb color);                    //получить моно цвет пикселя
    int getWidth() const;                               //получить ширину изображения (X)
    int getHeight() const;                              //получить высоту изображения (Y)
    QImage::Format getFormat() const;
    void setPixel(int x, int y, double color);          //задаёт цвет пикселя
    double getPixel(int x, int y) const;                //получить моно цвет
    QRgb getColorPixel(int x, int y);                   //получить цветной пиксель (нормализация)

    void swapPixel(int x1, int y1, int x2, int y2);     //поменять пиксели местами
    void horizontalSwap();                              //отобразить по горизонтали
    void verticalSwap();                                //отобразить по вертикали
    void blur(const MyQImage& originalImage);           //размытие
    void sharpness(const MyQImage& originalImage);      //резкость
    void sobel(
            QString Param,
            const MyQImage& originalImage
            );                                          //собель
    void gaussianFilter(
            const MyQImage& originalImage,
            double sigma
            );                                          //свёртка Гаусом
    void addNoise(int nPoint);                          //добавление шума
    void resizeImage(
            int newHeight,
            int newWidth
            );                                          //изменить размер изображения

    void Moravec(
            int _u,                                     //координаты в окне
            int _v,                                     //координаты в окне
            int _dx,                                    //сдвига окна по Х
            int _dy,                                    //сдвига окна по У
            int point_count,                            //кол-во интересных точек
            double T                                    //пороговое значение точки
            );                                          //оператор Моравека

    void Harris(
            int _dx,                                    //сдвига окна по Х
            int _dy,                                    //сдвига окна по У
            int point_count,                            //кол-во интересных точек
            double T,                                    //пороговое значение точки
            double k
            );

    void convolution(
            const MyQImage& originalImage,
            const double* kernel,
            int u,
            int v
            );                                          //свёртка

    void convolution(
            const MyQImage& originalImage,
            const double* row,
            const double* column,
            int u,
            int v
            );                                          //сепарабельная свёртка

    void saveImage(QString file);                       //сохранение изображение
};

#endif // MYQIMAGE_H
