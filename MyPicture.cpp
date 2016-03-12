#include "mypicture.h"

double MyPicture::GetSobelGradient(double Gx, double Gy)
{
    return sqrt((Gx*Gx) + (Gy*Gy));
}

void MyPicture::printKernel(double *Kernel, int width, int height)
{
    for(int x = 0 ; x < width; x++) {
        for(int y = 0; y < height; y++){
            printf("%lf ",  Kernel[x * width + y]);
        }
        printf("\n");
    }
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

void MyPicture::ResizeImage(int NewWidth, int NewHeight)
{
    Result.ResizeImage(NewWidth, NewHeight);
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

void MyPicture::Pyramid(int octave, double numLevel, double finishOctaveLevel)
{
    double k = pow(2.0, 1.0 / numLevel);
    double sigma;

    if(octave != 0){
        for(int i = 0; i < octave; i++){
            sigma = 1;
            for(double finish = 0; finish <= finishOctaveLevel && finish <= numLevel; finish++){

                if((sigma == 1 && i == 0) || (sigma != 1)){
                    GaussianFilter(sigma);
                }

                sigma *= k;

            }
            ResizeImage(Width() / 2, Height() / 2);
        }
    }
}

void MyPicture::GaussianFilter(double r)
{
    printf("Start: GaussianFilter (sigma = %lf)\n", r);

    if(r != 0) {
        int width_height = 6 * qRound(r) + 1;

        double Kernel[width_height][width_height];

        GetGaussianKernel(&Kernel[0][0], width_height, width_height, r);

        MyQImage copy = Result;

        Result.Convolution(copy, &Kernel[0][0], width_height, width_height);

    }

    printf("GaussianFilter OK!\n");
}

void MyPicture::GetGaussianKernel(double *Kernel, int width, int height, double r)
{
    double numPow;
    int x0, x1, y0, y1;
    x0 = - width / 2;
    x1 = width / 2 ;
    y0 = - height / 2;
    y1 = height / 2;

    for(double x = x0, xKernel = 0 ; x <= x1; x++, xKernel++) {
        for(double y = y0, yKernel = 0; y <= y1; y++, yKernel++){
            numPow = 0;
            numPow = -(x * x + y * y) / (2.0 * (r * r));
            Kernel[qRound(xKernel) * width + qRound(yKernel)] = pow(M_E, numPow) * (1.0 / (2.0 * M_PI * (r * r))) ;
        }
    }
}

void MyPicture::SavePictureInFile(QString file)
{
    Result.SaveImage(file);
}
