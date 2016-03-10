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

void MyPicture::HorizontalSwap()
{
    Result.HorizontalSwap();
}

void MyPicture::VerticalSwap()
{
    Result.VerticalSwap();
}

void MyPicture::Blur()
{
    printf("Start: Blur\n");
    double kernel[3][3] = {
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
    };

    Result.Convolution(&kernel[0][0], 3, 3);

    printf("Blur OK!\n");
}

void MyPicture::Sharpness()
{
    printf("Start: Sharpness\n");
    double kernel_blur[3][3] = {
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
    };

    double kernel_sharpness[3][3] = {
        {0.0, 0.0, 0.0},
        {0.0, 2.0, 0.0},
        {0.0, 0.0, 0.0}
    };

    double kernel[3][3] = {
        {-1.0 / 9.0, -1.0 / 9.0, -1.0 / 9.0},
        {-1.0 / 9.0, 17.0 / 9.0, -1.0 / 9.0},
        {-1.0 / 9.0, -1.0 / 9.0, -1.0 / 9.0}
    };


    Result.Convolution(&kernel[0][0], 3, 3);

    printf("Sharpness OK!\n");
}

void MyPicture::Sobel(QString Param)
{
    printf("Start: Sobel\n");

    double kernelX[3][3] = {
        {-1.0, 0.0, 1.0},
        {-2.0, 0.0, 2.0},
        {-1.0, 0.0, 1.0}
    };

    double kernelY[3][3] = {
        {-1.0,-2.0,-1.0},
        { 0.0, 0.0, 0.0},
        { 1.0, 2.0, 1.0}
    };

    double kernelAll[3][3] = {
        {sqrt(2), sqrt(4), sqrt(2)},
        {sqrt(8), sqrt(0), sqrt(8)},
        {sqrt(2), sqrt(4), sqrt(2)}
    };

    int u = 3,v = 3;

    MyQImage copy = Result;
    MyQImage copyX = Result;
    MyQImage copyY = Result;


    if(Param == "X") {
        Result.Convolution(copy, &kernelX[0][0],u,v);
    } else if(Param == "Y") {
        Result.Convolution(copy, &kernelY[0][0],u,v);
    } else if(Param == "All"){
        copyX.Convolution(copy, &kernelX[0][0],u,v);

        MyQImage copy1 = Result;
        copyY.Convolution(copy, &kernelY[0][0],u,v);

        for(int x = 0; x < Result.Height(); x++){
            for(int y = 0; y < Result.Width(); y++) {
                Result.SetPixel(x,y,GetSobelGradient(copyX.GetPixel(x,y),copyY.GetPixel(x,y)));
            }
        }
    }


    printf("Sobel OK!\n");
}

/*
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
*/

void MyPicture::SavePictureInFile(QString file)
{
    Result.SaveImage(file);
}
