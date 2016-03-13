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
    minOrigial = 9999.0;
    maxOrigial = -9999.0;

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
            }
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

int MyQImage::getNormalNumber(double Number) const
{
    return ((Number - min) / (max - min)) * (255);

}

double MyQImage::getMonoColor(QRgb color)
{
    double colorOrigial = ((double)qRed(color) * 0.299 + (double)qGreen(color) * 0.587 + (double)qBlue(color) * 0.114);

    if(colorOrigial > maxOrigial) maxOrigial = colorOrigial;
    if(colorOrigial < minOrigial) minOrigial = colorOrigial;

    return colorOrigial / 255.0;
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

QRgb MyQImage::GetColorPixel(int x, int y)
{
    double color = GetPixel(x,y);

    if(color != EXTRA_PIXEL){
        color = ((color - min) / (max - min)) * (maxOrigial - minOrigial);
        return qRgb(color, color, color);
    } else {
        count_interesting_point++;
        printf("*");
        return qRgb(255, 0, 0);
    }

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

void MyQImage::AddNoise(int count_point)
{
    printf("Add noise %d pieces\n", count_point);

    double max = GetPixel(0,0);
    double min = GetPixel(0,0);
    srand(time(NULL));

    for(int x = 0; x < Width()-1; x++){
        for(int y = 0; y < Height()-1; y++){

            if(max < GetPixel(x,y) && GetPixel(x,y) != EXTRA_PIXEL) max = GetPixel(x,y);
            if(min > GetPixel(x,y) && GetPixel(x,y) != EXTRA_PIXEL) min = GetPixel(x,y);
        }
    }

    for(int i = 0; i < count_point; i++){
        SetPixel(rand()%Width(), rand()%Height(), max);
    }
}

void MyQImage::ResizeImage(int NewWidth , int NewHeight)
{
    int stepX, stepY;
    stepY = Height() / NewHeight;
    stepX = Width() / NewWidth;
    double color;
    for(int NewX = 0, x = 0; NewX < NewWidth; NewX++, x += stepX){
        for(int NewY = 0, y = 0; NewY< NewHeight; NewY++, y += stepY){
            color = GetPixel(x,y);
            Image[NewX * NewWidth + NewY] = color;
        }
    }

    setHeight(NewHeight);
    setWidth(NewWidth);
}

void MyQImage::Convolution(const double *Kernel, int u, int v)
{
    int x0, x1, y0, y1;

    int xMax = Width()-1, yMax = Height()-1;

    double resultPixel = 0.0;
    int n = u * v - 1;

    for(int i = 0; i < Width(); i++){
        for(int j = 0; j <Height(); j++) {
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

    int xMax = image.Width()-1, yMax = image.Height()-1;

    double resultPixel = 0.0;
    int n = u * v - 1;

    for(int i = 0; i < image.Width(); i++){
        for(int j = 0; j < image.Height(); j++) {
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

    count_interesting_point = 0;

    auto dminmax = std::minmax(&Image[0], &Image[0] + Height()* Width() );

    max = GetPixel(0,0);
    min = GetPixel(0,0);
    min = *dminmax.second;
    max = *dminmax.first;

    max = GetPixel(0,0);
    min = GetPixel(0,0);

    for(int x = 0; x < Width()-1; x++){
        for(int y = 0; y < Height()-1; y++){

            if(max < GetPixel(x,y) && GetPixel(x,y) != EXTRA_PIXEL) max = GetPixel(x,y);
            if(min > GetPixel(x,y) && GetPixel(x,y) != EXTRA_PIXEL) min = GetPixel(x,y);
        }
    }

    for(int y = 0; y < Height(); y++){
        for(int x = 0; x < Width(); x++){
            Picture.setPixel(x, y, GetColorPixel(x,y));
        }
    }

    Picture.save(file);

    printf("\nSave file OK! (%d x %d)\nCount interesting point = %d\nmin = %lf; max = %lf \nminOriginal = %lf; maxOriginal = %lf\n",Width(), Height(),count_interesting_point,min, max, minOrigial, maxOrigial);
}

