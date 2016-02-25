#include "MyQImage.h"

void MyQImage::setWidth(int _width)
{
    width = _width;
}

void MyQImage::setHeight(int _height)
{
    height = _height;
}

void MyQImage::LoadImage(QString file)
{
    QImage Picture(file);
    format = Picture.format();

    if(Picture.width() != 0 && Picture.height() != 0){

        free(Image);

        setWidth(Picture.width());
        setHeight(Picture.height());

        Image = (double *)malloc(Width() * Height() * sizeof(double));

        if(Image == NULL){
            SetLoad(false);
            printf("Error: Insufficient memory available\n");
        } else {
            for(int x = 0; x < Height(); x++){
                for(int y = 0; y < Width(); y++){
                    SetPixel(x,y,getMonoColor(Picture.pixel(x,y)));
                }
            }

            SetLoad(true);
            printf("Load succesfully!\n");
        }
    } else {
        SetLoad(false);
        printf("Error: file not load!\n");
    }
}

MyQImage::MyQImage(QString file)
{
    LoadImage(file);
}

MyQImage::MyQImage()
{
    SetLoad(false);
    Image = NULL;
}

double MyQImage::checkColor(double color)
{
    if(color > 255.0) color = 255.0;
    if(color < 0.0) color = 0.0;
    return color;
}

double MyQImage::getMonoColor(QRgb color)
{
    return (double)qRed(color) * 0.299 + (double)qGreen(color) * 0.587 + (double)qBlue(color) * 0.114;
}

int MyQImage::Width()
{
    return width;
}

int MyQImage::Height()
{
    return height;
}

bool MyQImage::IsLoad()
{
    return load;
}

void MyQImage::SetLoad(bool Load)
{
    load = Load;
}

void MyQImage::SetPixel(int x, int y, double color)
{
    *(Image + x * Width() + y) = checkColor(color);
}

double MyQImage::GetPixel(int x, int y)
{
    return *(Image + x * Width() + y);
}

QRgb MyQImage::GetColorPixel(int x, int y)
{
    return qRgb(GetPixel(x,y),GetPixel(x,y),GetPixel(x,y));
}

void MyQImage::SwapPixel(int x1, int y1, int x2, int y2)
{
    double pixel = GetPixel(x1,y1);
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

void MyQImage::SaveImage(QString file)
{
    if(IsLoad()) {
        QImage Picture(Width(),Height(),format);

        for(int x = 0; x < Height(); x++){
            for(int y = 0; y < Width(); y++){
                Picture.setPixel(x,y,GetColorPixel(x,y));
            }
        }

        Picture.save(file);

        printf("Save file OK!\n");
    }
}

