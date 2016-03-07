#include "mypicture.h"

double MyPicture::GetSobelGradient(double Gx, double Gy)
{
    return sqrt((Gx*Gx) + (Gy*Gy));
}

int MyPicture::Width() const
{
    return Result.Width();
}

int MyPicture::Height() const
{
    return Result.Height();
}

MyPicture::MyPicture(QString file){
    Result.LoadImage(file);
}

void MyPicture::ConvertToNegative()
{
    printf("Start: ConvertToNegative\n");
    for(int x = 0; x < Result.Height(); x++){
        for(int y = 0; y < Result.Width(); y++) {
            Result.SetPixel(x,y,1.0 - Result.GetPixel(x,y));
        }
    }
    printf("ConvertToNegative OK!\n");
}

int MyPicture::reflect(int x, int MaxX)
{
    if(x >= MaxX){
        return MaxX - (x - MaxX) - 1;
    }

    if(x < 0){
        return -1 * x;
    }

    return x;
}

void MyPicture::HorizontalSwap()
{
    Result.HorizontalSwap();
}

void MyPicture::WerticalSwap()
{
    Result.WerticalSwap();
}

void MyPicture::Blur()
{
    printf("Start: Blur\n");
    int kernel[3][3] = {
        {1,1,1},
        {1,1,1},
        {1,1,1}
    };
    double color;

    for(int x = 0; x < Result.Height() - 1; x++){
        for(int y = 0; y < Result.Width() - 1; y++) {
            color = Convolution(&kernel[0][0],1,1,x,y)/9;
            Result.SetPixel(x,y,color);
        }
    }
    printf("Blur OK!\n");
}

void MyPicture::Sharpness()
{
    printf("Start: Sharpness\n");
    int kernel_blur[3][3] = {
        {1,1,1},
        {1,1,1},
        {1,1,1}
    };

    int kernel_sharpness[3][3] = {
        {0,0,0},
        {0,2,0},
        {0,0,0}
    };

    double color;

    for(int x = 0; x < Result.Height()-1; x++){
        for(int y = 0; y < Result.Width()-1; y++) {
            color = Convolution(&kernel_sharpness[0][0],1,1,x,y) - Convolution(&kernel_blur[0][0],1,1,x,y)/9;
            Result.SetPixel(x,y,color);
        }
    }
    printf("Sharpness OK!\n");
}

void MyPicture::Sobel(QString Param)
{
    printf("Start: Sobel\n");

    int kernelX[3][3] = {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };

    int kernelY[3][3] = {
        {-1,-2,-1},
        { 0, 0, 0},
        { 1, 2, 1}
    };

    double color;
    double Gx,Gy;

    MyQImage copy = Result;

    for(int x = 0; x < Result.Height(); x++){
        for(int y = 0; y < Result.Width(); y++) {
            if(Param == "X") {
                color = Convolution(copy, &kernelX[0][0],1,1,x,y);
                Result.SetPixel(x,y,color);
            } else if(Param == "Y") {
                color = Convolution(copy, &kernelY[0][0],1,1,x,y);
                Result.SetPixel(x,y,color);
            } else if(Param == "All"){
                Gx = Convolution(copy, &kernelX[0][0],1,1,x,y);
                Gy = Convolution(copy, &kernelY[0][0],1,1,x,y);
                Result.SetPixel(x,y,GetSobelGradient(Gx,Gy));
            }

        }
    }

    printf("Sobel OK!\n");
}

void MyPicture::GaussianFilter(double _r)
{
    printf("Start: GaussianFilter\n");
    double color;

    if(_r != 0){
        for(int x = 0; x < Result.Height(); x++){
            for(int y = 0; y < Result.Width(); y++) {
                color = GaussianConvolution(x, y, _r);
                Result.SetPixel(x,y,color);
            }
        }
    }

    printf("GaussianFilter OK!\n");
}

double MyPicture::GaussianConvolution(int _x, int _y, double r)
{
    //int x0 = _x - _u, x1 = _x + _u, y0 = _y - _v, y1 = _y + _v;
    int xMax = Result.Height(), yMax = Result.Width();

    double resultPixel = 0.0;
    double numPow;

    for(double u = -r ; u < r + 1; u++) {
        for(double v = -r; v < r + 1; v++){
            if((r * r) >= (u * u + v * v)){
                numPow = 0;
                numPow = -(u * u + v * v) / (2.0 * (r * r));
                resultPixel += (pow(M_E, numPow) * Result.GetPixel(reflect(_x + u, xMax),reflect(_y + v, yMax))) * (1.0 / (2.0 * M_PI * (r * r))) ;
            }
        }
    }
    return resultPixel * 2.3;
}

double MyPicture::Convolution(const int *Kernel, int u, int v, int x, int y)
{
    int x0 = x - u, x1 = x + u, y0 = y - v, y1 = y + v;
    int xMax = Result.Height()-1, yMax = Result.Width()-1;

    double resultPixel = 0.0;
    int n = 8;

    for(int y = y0 ; y <= y1; y++) {
        for(int x = x0; x <= x1; x++, n--){
            //printf("Kernel[%d] = %lf; reflect_x = %d; reflect_y = %d;GetPixel = %lf\n",n,(double)Kernel[n],reflect(x, xMax),reflect(y, yMax),Result.GetPixel(reflect(x, xMax),reflect(y, yMax)));
            resultPixel += ((double)Kernel[n] * Result.GetPixel(reflect(x, xMax),reflect(y, yMax)));
        }
    }
    return resultPixel;
}

double MyPicture::Convolution(const MyQImage& image, const int *Kernel, int u, int v, int x, int y)
{
    int x0 = x - u, x1 = x + u, y0 = y - v, y1 = y + v;
    int xMax = image.Height(), yMax = image.Width();

    double resultPixel = 0.0;
    int n = 8;

    for(int y = y0 ; y <= y1; y++) {
        for(int x = x0; x <= x1; x++, n--){
            resultPixel += ((double)Kernel[n] * image.GetPixel(reflect(x, xMax),reflect(y, yMax)));
        }
    }
    return resultPixel;
}

void MyPicture::SavePictureInFile(QString file)
{
    Result.SaveImage(file);
}
