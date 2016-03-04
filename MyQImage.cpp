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

        setWidth(Picture.width());
        setHeight(Picture.height());

        Image = std::make_unique<double[]>(Width() * Height());


        for(int x = 0; x < Height(); x++){
            for(int y = 0; y < Width(); y++){
                SetPixel(x,y,getMonoColor(Picture.pixel(x,y)));
            }
        }

        printf("Load succesfully! (%d x %d)\n",Width(), Height());
    } else {
        printf("Error: file not load!\n");
        exit(-1);
    }
}

MyQImage::MyQImage(QString file)
{
    LoadImage(file);
}

MyQImage::MyQImage()
{
    Image = NULL;
}

MyQImage::MyQImage(const MyQImage& data)
{
    if(data.Width() != 0 && data.Height() != 0){
        Image = std::make_unique<double[]>(data.Width() * data.Height());
        setWidth(data.Width());
        setHeight(data.Height());
        format = data.format;

        for(int x = 0; x < data.Height(); x++){
            for(int y = 0; y < data.Width(); y++){
                SetPixel(x,y,data.GetPixel(x,y));
                //printf("x = %d; y = %d; data = %lf; this = %lf\n", x,y,data.GetPixel(x,y),GetPixel(x,y));
            }
           // printf("x = %d;\n", x);
        }

        printf("Copy succesfully!\n");
    } else {
        printf("Error: Copy data is bad!\n");
        exit(-1);
    }
}

MyQImage::~MyQImage()
{
    printf("Memory free\n");
}

double MyQImage::checkColor(double color)
{
    if(color > 1.0) color = 1.0;
    if(color < 0.0) color = 0.0;
    return color;
}

double MyQImage::getMonoColor(QRgb color)
{
    return ((double)qRed(color) * 0.299 + (double)qGreen(color) * 0.587 + (double)qBlue(color) * 0.114) / 255.0;
}

int MyQImage::Width() const
{
    return width;
}

int MyQImage::Height() const
{
    return height;
}

void MyQImage::SetPixel(int x, int y, double color)
{
    Image[x * Width() + y] = checkColor(color);
}

double MyQImage::GetPixel(int x, int y) const
{
    return Image[x * Width() + y];
}

QRgb MyQImage::GetColorPixel(int x, int y)
{
    return qRgb(GetPixel(x,y) * 255,GetPixel(x,y) * 255,GetPixel(x,y) * 255);
}

void MyQImage::SwapPixel(int x1, int y1, int x2, int y2)
{
    double pixel = GetPixel(x1,y1);
    SetPixel(x1, y1, GetPixel(x2,y2));
    SetPixel(x2, y2, pixel);
}

void MyQImage::HorizontalSwap()
{
    printf("Start: HorizontalSwap\n");
    for(int x = 0; x < Height(); x++){
        for(int y = 0; y < Width()/2; y++) {
            SwapPixel(x, y, x, (Width()-1) - y);
        }
    }
    printf("HorizontalSwap OK!\n");
}

void MyQImage::WerticalSwap()
{
    printf("Start: WerticalSwap\n");
    for(int x = 0; x < Height()/2; x++){
        for(int y = 0; y < Width(); y++) {
            SwapPixel(x, y,(Height() - 1) - x, y);
        }
    }
    printf("WerticalSwap OK!\n");
}

void MyQImage::SaveImage(QString file)
{
    QImage Picture(Width(),Height(),format);

    for(int x = 0; x < Height(); x++){
        for(int y = 0; y < Width(); y++){
            Picture.setPixel(x,y,GetColorPixel(x,y));
        }
    }

    Picture.save(file);

    printf("Save file OK!\n");
}

