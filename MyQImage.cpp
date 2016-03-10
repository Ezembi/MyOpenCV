#include "MyQImage.h"

void MyQImage::setWidth(int _width)
{
    width = _width;
}

void MyQImage::setHeight(int _height)
{
    height = _height;
}

int MyQImage::reflect(int x, int MaxX)
{
    if(x >= MaxX){
        return MaxX - (x - MaxX) - 1;
    }

    if(x < 0){
        return -1 * x;
    }

    return x;
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

void MyQImage::getNormalStep()
{
    //  auto dminmax = std::minmax(Image);
    //  return ((double)dminmax.second - (double)dminmax.first) / 255.0;

    min = GetPixel(0,0), max = GetPixel(0,0);
    for(int x = 0; x < Height(); x++){
        for(int y = 0; y < Width(); y++){
            if(GetPixel(x,y) > max) max = GetPixel(x,y);
            if(GetPixel(x,y) < min) min = GetPixel(x,y);
        }
    }


    step = (max - min) / 255.0;
}

int MyQImage::getNormalNumber(double Number) const
{
    int normalNumber = 0;
    double i = min;
    while (i < Number && i < max){
        normalNumber++;
        i += step;
    }

    return normalNumber;
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
    Image[x * Width() + y] = color;
}

double MyQImage::GetPixel(int x, int y) const
{
    return Image[x * Width() + y];
}

QRgb MyQImage::GetColorPixel(int x, int y) const
{
    double color = getNormalNumber(GetPixel(x,y));
    return qRgb(color, color, color);
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

void MyQImage::VerticalSwap()
{
    printf("Start: VerticalSwap\n");
    for(int x = 0; x < Height()/2; x++){
        for(int y = 0; y < Width(); y++) {
            SwapPixel(x, y,(Height() - 1) - x, y);
        }
    }
    printf("VerticalSwap OK!\n");
}

void MyQImage::Convolution(const double *Kernel, int u, int v)
{
    int x0, x1, y0, y1;

    int xMax = Height()-1, yMax = Width()-1;

    double resultPixel = 0.0;
    int n = u * v - 1;

    for(int i = 0; i < Height()-1; i++){
        for(int j = 0; j < Width()-1; j++) {
            x0 = i - (u / 2);
            x1 = i + (u / 2);
            y0 = j - (v / 2);
            y1 = j + (v / 2);
            resultPixel = 0.0;
            n = u * v - 1;

            for(int y = y0 ; y <= y1; y++) {
                for(int x = x0; x <= x1; x++, n--){
                    resultPixel += (Kernel[n] * GetPixel(reflect(x, xMax),reflect(y, yMax)));
                }
            }

            SetPixel(i,j,resultPixel);

        }
    }
}

void MyQImage::Convolution(const MyQImage& image, const double *Kernel, int u, int v)
{
    int x0, x1, y0, y1;

    int xMax = image.Height()-1, yMax = image.Width()-1;

    double resultPixel = 0.0;
    int n = u * v - 1;

    for(int i = 0; i < image.Height()-1; i++){
        for(int j = 0; j < image.Width()-1; j++) {
            x0 = i - (u / 2);
            x1 = i + (u / 2);
            y0 = j - (v / 2);
            y1 = j + (v / 2);
            resultPixel = 0.0;
            n = u * v - 1;

            for(int y = y0 ; y <= y1; y++) {
                for(int x = x0; x <= x1; x++, n--){
                    resultPixel += (Kernel[n] * image.GetPixel(reflect(x, xMax),reflect(y, yMax)));
                }
            }

            SetPixel(i,j,resultPixel);

        }
    }
}

void MyQImage::SaveImage(QString file)
{
    QImage Picture(Width(),Height(),format);

    getNormalStep();

    for(int x = 0; x < Height(); x++){
        for(int y = 0; y < Width(); y++){
            Picture.setPixel(x,y,GetColorPixel(x,y));
        }
    }

    Picture.save(file);

    printf("Save file OK!\n");
}

