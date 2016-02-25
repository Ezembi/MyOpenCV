#include "mypicture.h"

void MyPicture::SetLoad(bool _load)
{
    load = _load;
}

int MyPicture::getN(int x0, int x1, int y0, int y1)
{
    return (((x1 - x0)+1) * ((y1 - y0)+1)) - 1;
}

double *MyPicture::Clone(MyQImage image)
{
    double * result;

    if(image.IsLoad()){
        free(result);
        result = (double *)malloc(Width() * Height() * sizeof(double));
        if(result == NULL){
            SetLoad(false);
            printf("Error: Insufficient memory available\n");
        } else {
            for(int x = 0; x < Height(); x++){
                for(int y = 0; y < Width(); y++){
                    *(result + x * Width() + y) = image.GetPixel(x,y);
                }
            }

            SetLoad(true);
            printf("Load succesfully!\n");
        }
    }

    return result;
}

double MyPicture::GetSobelGradient(double Gx, double Gy)
{
    return sqrt((Gx*Gx) + (Gy*Gy));
}

int MyPicture::Width()
{
    return Result.Width();
}

int MyPicture::Height()
{
    return Result.Height();
}

bool MyPicture::IsLoad()
{
    return Result.IsLoad();
}

MyPicture::MyPicture(QString file){
    Result.LoadImage(file);
    SetLoad(Result.IsLoad());
}

void MyPicture::ConvertToNegative()
{
    if(IsLoad()) {
        printf("Start: ConvertToNegative\n");
        for(int x = 0; x < Result.Height(); x++){
            for(int y = 0; y < Result.Width(); y++) {
                Result.SetPixel(x,y,255.0 - Result.GetPixel(x,y));
            }
        }
        printf("ConvertToNegative OK!\n");
    }
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
    if(IsLoad()) {
        printf("Start: Blur\n");
        int kernel[3][3] = {
            {1,1,1},
            {1,1,1},
            {1,1,1}
        };
        double color;

        for(int x = 0; x < Result.Height(); x++){
            for(int y = 0; y < Result.Width(); y++) {
                color = Convolution(&kernel[0][0],1,1,x,y)/9;
                Result.SetPixel(x,y,color);
            }
        }
        printf("Blur OK!\n");
    }
}

void MyPicture::Sharpness()
{
    if(IsLoad()) {
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

        for(int x = 0; x < Result.Height(); x++){
            for(int y = 0; y < Result.Width(); y++) {
                color = Convolution(&kernel_sharpness[0][0],1,1,x,y) - Convolution(&kernel_blur[0][0],1,1,x,y)/9;
                Result.SetPixel(x,y,color);
            }
        }
        printf("Sharpness OK!\n");
    }
}

void MyPicture::Sobel(QString Param)
{
    if(IsLoad()) {
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

        double * period = Clone(Result);

        for(int x = 0; x < Result.Height(); x++){
            for(int y = 0; y < Result.Width(); y++) {
                if(Param == "X") {
                    color = Convolution(period, &kernelX[0][0],1,1,x,y);
                    Result.SetPixel(x,y,color);
                } else if(Param == "Y") {
                    color = Convolution(period, &kernelY[0][0],1,1,x,y);
                    Result.SetPixel(x,y,color);
                } else if(Param == "All"){
                    Gx = Convolution(period, &kernelX[0][0],1,1,x,y);
                    Gy = Convolution(period, &kernelY[0][0],1,1,x,y);
                    color = GetSobelGradient(Gx,Gy);
                    Result.SetPixel(x,y,color);
                }

            }
        }

        free(period);

        printf("Sobel OK!\n");
    }
}

double MyPicture::Convolution(int *Kernel, int u, int v, int x, int y)
{
    int x0 = x - u, x1 = x + u, y0 = y - v, y1 = y + v;
    if(x0 < 0){
        x0 = 0;
    }
    if(x1 >= Result.Width()){
        x1 = Result.Width() - 1;
    }
    if(y0 < 0){
        y0 = 0;
    }
    if(y1 >= Result.Height()){
        y1 = Result.Height() - 1;
    }

    double resultPixel = 0.0;
    int n = getN(x0,x1,y0,y1);

    for(int y = y0 ; y <= y1; y++) {
        for(int x = x0; x <= x1; x++, n--){
            resultPixel += (double)(Kernel[n] * Result.GetPixel(x,y));
        }
    }
    return resultPixel;
}

double MyPicture::Convolution(double * image, int *Kernel, int u, int v, int x, int y)
{
    int x0 = x - u, x1 = x + u, y0 = y - v, y1 = y + v;
    if(x0 < 0){
        x0 = 0;
    }
    if(x1 >= Result.Width()){
        x1 = Result.Width() - 1;
    }
    if(y0 < 0){
        y0 = 0;
    }
    if(y1 >= Result.Height()){
        y1 = Result.Height() - 1;
    }

    double resultPixel = 0.0;
    int n = getN(x0,x1,y0,y1);

    for(int y = y0 ; y <= y1; y++) {
        for(int x = x0; x <= x1; x++, n--){
            resultPixel += (double)(Kernel[n] * *(image + x * Width() + y));
        }
    }
    return resultPixel;
}

void MyPicture::SavePictureInFile(QString file)
{
    if(IsLoad()) {
        Result.SaveImage(file);
        printf("Save file OK!\n");
    }
}
