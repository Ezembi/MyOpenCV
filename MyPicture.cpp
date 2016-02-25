#include "mypicture.h"

void MyPicture::SetLoad(bool _load)
{
    load = _load;
}

int MyPicture::getMonoColor(QRgb color)
{
    return qRed(color) * 0.299 + qGreen(color) * 0.587 + qBlue(color) * 0.114;
}

QRgb MyPicture::getMonoPixel(QRgb color)
{
    return qRgb(getMonoColor(color), getMonoColor(color),getMonoColor(color));
}

QRgb MyPicture::getCustomPixel(int Red, int Green, int Blue)
{
    return qRgb(Red, Green, Blue);
}

int MyPicture::getN(int x0, int x1, int y0, int y1)
{
    return (((x1 - x0)+1) * ((y1 - y0)+1)) - 1;
}

QRgb MyPicture::GetSobelGradient(QRgb Gx, QRgb Gy)
{
    double Red, Green, Blue;
    Red = sqrt(qRed(Gx) * qRed(Gx) + qRed(Gy) * qRed(Gy));
    Green = sqrt(qGreen(Gx) * qGreen(Gx) + qGreen(Gy) * qGreen(Gy));
    Blue = sqrt(qBlue(Gx) * qBlue(Gx) + qBlue(Gy) * qBlue(Gy));

    return getCustomPixel(Red, Green, Blue);
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
    Original.LoadImage(file);
    Result.LoadImage(file);
}

void MyPicture::ResetToOriginal()
{
    if(IsLoad()) {
        for(int x = 0; x < Original.Height(); x++){
            for(int y = 0; y < Original.Width(); y++) {
                Result.SetPixel(x,y,Original.GetPixel(x,y));
            }
        }
    }
}

double MyPicture::checkColor(double color)
{
    if(color > 255) color = 255;
    if(color < 0) color = 0;
    return color;
}

void MyPicture::ConvertToMonochrome()
{
    if(IsLoad()) {
        printf("Start: ConvertToMonochrome\n");
        for(int i = 0; i < Original.Height(); i++){
            for(int j = 0; j < Original.Width(); j++) {
                Result.SetPixel(i,j,getMonoPixel(Result.GetPixel(i,j)));
            }
        }
        printf("ConvertToMonochrome OK!\n");
    }
}

void MyPicture::ConvertToNegative()
{
    if(IsLoad()) {
        printf("Start: ConvertToNegative\n");
        for(int x = 0; x < Original.Height(); x++){
            for(int y = 0; y < Original.Width(); y++) {
                Result.SetPixel(x,y,getCustomPixel(255 - Result.getRed(x,y),255 - Result.getGreen(x,y),255 - Result.getBlue(x,y)));
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
                Result.SetPixel(x,y,getCustomPixel(color,color,color));
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
                Result.SetPixel(x,y,getCustomPixel(color,color,color));
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

        QRgb color;
        QRgb Gx,Gy;

        MyQImage period(Result.GetImage());

        for(int y = 0; y < Result.Width(); y++) {
            for(int x = 0; x < Result.Height(); x++){

                if(Param == "X") {
                    color = ConvolutionColor(period,&kernelX[0][0],1,1,x,y);
                    Result.SetPixel(x,y,color);
                } else if(Param == "Y") {
                    color = ConvolutionColor(period,&kernelY[0][0],1,1,x,y);
                    Result.SetPixel(x,y,color);
                } else if(Param == "All"){
                    Gx = ConvolutionColor(period,&kernelX[0][0],1,1,x,y);
                    Gy = ConvolutionColor(period,&kernelY[0][0],1,1,x,y);
                    color = GetSobelGradient(Gx,Gy);
                    Result.SetPixel(x,y,color);
                }

            }
        }
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
            resultPixel += (double)(Kernel[n] * getMonoColor(Result.GetPixel(x,y)));
        }
    }
    return resultPixel;
}

double MyPicture::Convolution(MyQImage image, int *Kernel, int u, int v, int x, int y)
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
            resultPixel += (double)(Kernel[n] * getMonoColor(image.GetPixel(x,y)));
        }
    }
    return resultPixel;
}

QRgb MyPicture::ConvolutionColor(int *Kernel, int u, int v, int x, int y)
{
    double red = 0.0, green = 0.0, blue = 0.0;

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

    int n = getN(x0,x1,y0,y1);
    //n = 0;

    double r,g,b,k;

    for(int y = y0 ; y <= y1; y++) {
        for(int x = x0; x <= x1; x++, n--){
            r = Result.getRed(x,y);
            g = Result.getGreen(x,y);
            b = Result.getBlue(x,y);
            k = Kernel[n];

            red += (double)(k * r);
            green += (double)(k * g);
            blue += (double)(k * b);
        }
    }

    red = checkColor(red);
    green = checkColor(green);
    blue = checkColor(blue);

    return getCustomPixel(red,green,blue);
}

QRgb MyPicture::ConvolutionColor(MyQImage image, int *Kernel, int u, int v, int x, int y)
{
    double red = 0.0, green = 0.0, blue = 0.0;

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

    int n = getN(x0,x1,y0,y1);
    //n = 0;

    double r,g,b,k;

    for(int y = y0 ; y <= y1; y++) {
        for(int x = x0; x <= x1; x++, n--){
            r = image.getRed(x,y);
            g = image.getGreen(x,y);
            b = image.getBlue(x,y);
            k = Kernel[n];

            red += (double)(k * r);
            green += (double)(k * g);
            blue += (double)(k * b);
        }
    }

    red = checkColor(red);
    green = checkColor(green);
    blue = checkColor(blue);

    return getCustomPixel(red,green,blue);
}

void MyPicture::SavePictureInFile(QString file)
{
    if(IsLoad()) {
        Result.Save(file);
        printf("Save file OK!\n");
    }
}
