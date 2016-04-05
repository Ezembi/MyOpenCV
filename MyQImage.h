#ifndef MYQIMAGE_H
#define MYQIMAGE_H
#include <GlobalNames.h>
#include <time.h>
#include <QImage>
#include <QPainter>
#include <QRgb>
#include <QString>
#include <memory>
#include <algorithm>
#include <assistant.h>

struct InterestingPoint
{
    int x_, y_;
    double value_;

    InterestingPoint()
    {

    }

    InterestingPoint(int x, int y, double value)
    {
        x_ = x;
        y_ = y;
        value_ = value;
    }

    void print(){
        printf("x = %d, y = %d, value = %lf\n", x_, y_, value_);
    }

    double distance(InterestingPoint p){
        return sqrt((p.x_ - x_) * (p.x_ - x_) + (p.y_ - y_) * (p.y_ - y_));
    }

};

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

    MyQImage getSobelGradient(
            const MyQImage& xImage,
            const MyQImage& yImage
            ) const;                                    //получить значение градиента собеля


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

    MyQImage(MyQImage &&image);

    ~MyQImage();

    const MyQImage& operator=(MyQImage &&image);
    const MyQImage& operator=(const MyQImage &image);

    int getNormalNumber(double number) const;           //получить нормализованное число
    double getMonoColor(QRgb color);                    //получить моно цвет пикселя
    int getWidth() const;                               //получить ширину изображения (X)
    int getHeight() const;                              //получить высоту изображения (Y)
    QImage::Format getFormat() const;
    void setPixel(int x, int y, double color);          //задаёт цвет пикселя
    double getPixel(int x, int y) const;                //получить моно цвет
    QRgb getColorPixel(int x, int y) const;                   //получить цветной пиксель (нормализация)
    void copy(const MyQImage& imageForCopy);            //скопировать изображение в данное
    void resizeAndCopy(
            const MyQImage& imageForCopy,
            int newWidth,
            int newHeight
            );                                          //скопировать изображение в данное с исзменённым размером


    void swapPixel(int x1, int y1, int x2, int y2);     //поменять пиксели местами
    void horizontalSwap();                              //отобразить по горизонтали
    void verticalSwap();                                //отобразить по вертикали
    MyQImage blur() const;                              //размытие
    MyQImage sharpness() const;                         //резкость
    MyQImage sobel(QString Param) const;                //собель (!ГРАДИЕНТ!)

    MyQImage gaussianFilter(
            double sigma
            ) const;                                    //свёртка Гаусом

    void addNoise(int nPoint);                          //добавление шума

    std::vector<InterestingPoint> Moravec(
            int _u,                                     //координаты в окне
            int _v,                                     //координаты в окне
            int _dx,                                    //сдвига окна по Х
            int _dy,                                    //сдвига окна по У
            double T                                    //пороговое значение точки
            ) const;                                          //оператор Моравека

    std::vector<InterestingPoint> Harris(
            int _dx,                                    //сдвига окна по Х
            int _dy,                                    //сдвига окна по У
            double T,                                   //пороговое значение точки
            double k
            ) const;

    static std::vector<InterestingPoint> ANMS(
            std::vector<InterestingPoint> points,
            int nPoint
            );                                          //Adaptive non-maximum suppression

    MyQImage convolution(
            const double* kernel,
            int u,
            int v
            ) const;                                    //свёртка

    MyQImage convolution(
            const double* row,
            const double* column,
            int u,
            int v
            ) const;                                    //сепарабельная свёртка

    QImage getQImage();

    void saveImage(QString file);                       //сохранение изображение
    void saveImage(
            QString file,
            std::vector<InterestingPoint> points
            );                                          //сохранение изображение с интересными очками
};

#endif // MYQIMAGE_H
