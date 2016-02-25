#include "MyQImage.h"

MyQImage::MyQImage(QString file)
{
    Image = QImage(file);
    if(Image.width() != 0 && Image.height() != 0){
        SetLoad(true);
        printf("Load succesfully!\n");
    } else {
        SetLoad(false);
        printf("Error: file not load!\n");
    }
}

MyQImage::MyQImage(QImage Parent)
{
    QImage period(Parent.width(),Parent.height(), Parent.format());
    Image = Parent;
    if(Image.width() != 0 && Image.height() != 0){
        SetLoad(true);
        printf("Load succesfully!\n");
    } else {
        SetLoad(false);
        printf("Error: file not load!\n");
    }
}

MyQImage::MyQImage()
{
    SetLoad(false);
}

void MyQImage::LoadImage(QString file)
{
    Image = QImage(file);
    if(Image.width() != 0 && Image.height() != 0){
        SetLoad(true);
        printf("Load succesfully!\n");
    } else {
        SetLoad(false);
        printf("Error: file not load!\n");
    }
}

QImage MyQImage::GetImage()
{
    return Image;
}

int MyQImage::Width()
{
    return Image.width();
}

int MyQImage::Height()
{
    return Image.height();
}

bool MyQImage::IsLoad()
{
    return load;
}

void MyQImage::SetLoad(bool Load)
{
    load = Load;
}

void MyQImage::SetPixel(int x, int y, QRgb color)
{
    Image.setPixel(x,y,color);
}

QRgb MyQImage::GetPixel(int x, int y)
{
    return Image.pixel(x,y);
}

int MyQImage::getRed(int x, int y)
{
    return qRed(Image.pixel(x,y));
}

int MyQImage::getGreen(int x, int y)
{
    return qGreen(Image.pixel(x,y));
}

int MyQImage::getBlue(int x, int y)
{
    return qBlue(Image.pixel(x,y));
}

void MyQImage::SwapPixel(int x1, int y1, int x2, int y2)
{
    QRgb pixel = GetPixel(x1,y1);
    SetPixel(x1, y1, GetPixel(x2,y2));
    SetPixel(x2, y2, pixel);
}

void MyQImage::HorizontalSwap()
{
    if(IsLoad()) {
        printf("Start: HorizontalSwap\n");
        for(int x = 0; x < Height(); x++){
            for(int y = 0; y < Width()/2; y++) {
                SwapPixel(x, y, x, (Width()-1) - y);
            }
        }
        printf("HorizontalSwap OK!\n");
    }
}

void MyQImage::WerticalSwap()
{
    if(IsLoad()) {
        printf("Start: WerticalSwap\n");
        for(int x = 0; x < Height()/2; x++){
            for(int y = 0; y < Width(); y++) {
                SwapPixel(x, y,(Height() - 1) - x, y);
            }
        }
        printf("WerticalSwap OK!\n");
    }
}

void MyQImage::Save(QString file)
{
    if(IsLoad()) {
        Image.save(file);
        printf("Save file OK!\n");
    }
}

