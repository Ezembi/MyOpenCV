#ifndef MYQIMAGE_H
#define MYQIMAGE_H
#include <QImage>
#include <QRgb>
#include <QString>

class MyQImage
{
private:
    QImage Image;
    bool load;

public:
    MyQImage(QString file);
    MyQImage(QImage Parent);
    MyQImage();

    void LoadImage(QString file);                   //загрузка изображения
    QImage GetImage();                              //получить изображение
    int Width();                                    //получить ширину изображения
    int Height();                                   //получить высоту изображения
    bool IsLoad();                                  //проверна на загруженность изображения
    void SetLoad(bool Load);                        //загрузка изображения
    void SetPixel(int x, int y, QRgb color);        //задаёт цвет пикселя
    QRgb GetPixel(int x, int y);                    //получить цвет пикселя
    int getRed(int x, int y);                       //получить значение красного цвета пикселя
    int getGreen(int x, int y);                     //получить значение зелёного цвета пикселя
    int getBlue(int x, int y);                      //получить значение синего цвета пикселя
    void SwapPixel(int x1, int y1, int x2, int y2); //поменять пиксели местами
    void HorizontalSwap();                          //отобразить по горизонтали
    void WerticalSwap();                            //отобразить по вертикали
    void Save(QString file);                        //сохранение изображения
};

#endif // MYQIMAGE_H
