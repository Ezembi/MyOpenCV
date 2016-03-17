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

int MyPicture::ANMS(const double *Array_interesting_point, int point_count, double T)
{
    int local_count;
    int radius = 0;
    bool isMax;
    int maxX = Width(), maxY = Height();

    do{
        radius++;
        local_count = 0;

        for(int x = 0; x < Width(); x++){
            for(int y = 0; y < Height(); y++){
                isMax = true;
                for(int px = -radius; px < radius; px++){
                    for(int py = -radius; py < radius; py++){
                        if(Array_interesting_point[x * Width() + y] < Array_interesting_point[reflect(x + px, maxX) * Width() + reflect(y + py, maxY)]){
                            isMax = false;
                        }
                    }
                }

                if(isMax && Array_interesting_point[x * Width() + y] > T){
                    local_count++;
                }
            }
        }
    }while(local_count > point_count);

    return radius;
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

void MyPicture::AddNoise(int count_point)
{
    Result.AddNoise(count_point);
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

void MyPicture::Moravec(int _u, int _v, int _dx, int _dy, int point_count, double T)
{
    printf("Start: Moravec\n");
    std::unique_ptr<double[]> S = std::make_unique<double[]>(Width() * Height());   //значение оператора

    int u0, u1, v0, v1;
    double minC;
    double C;
    double color1, color2;
    int maxX = Width(), maxY = Height();


    for(int x = 0; x < Width(); x++){
        for(int y = 0; y < Height(); y++){
            u0 = x - _u / 2;
            u1 = x + _u / 2;
            v0 = y - _v / 2;
            v1 = y + _v / 2;

            for(int dx = -_dx; dx < _dx; dx++){
                for(int dy = -_dy; dy < _dy; dy++){
                    C = 0;
                    minC = -1;
                    /*if(dx == 0 && dy == 0){} else*/ {
                        for(int u = u0; u < u1; u++){
                            for(int v = v0; v < v1; v++){

                                color1 = Result.GetPixel(reflect(u, maxX), reflect(v, maxY));
                                color2 = Result.GetPixel(reflect(u + dx, maxX), reflect(v + dy, maxY));
                                C += (color1 - color2) * (color1 - color2);

                            }
                        }
                        if(C < minC) minC = C;
                    }
                }
            }
            S[x * Width() + y] = C;
        }
    }

    bool isMax;

    int _px, _py;

    _px = _py = ANMS(&S[0], point_count, T);

    for(int x = 0; x < Width(); x++){
        for(int y = 0; y < Height(); y++){
            isMax = true;
            for(int px = -_px; px < _px; px++){
                for(int py = -_py; py < _py; py++){
                    if(S[x * Width() + y] < S[reflect(x + px, maxX) * Width() + reflect(y + py, maxY)]){
                        isMax = false;
                    }
                }
            }

            if(isMax && S[x * Width() + y] > T){
                Result.SetPixel(x,y,EXTRA_PIXEL);
            }
        }
    }

    printf("Moravec OK!\n");
}

void MyPicture::Harris()
{
    /*
     * что такое градиент?
     * что такое Ix? производная? чего?
     * А - ? Сумма по оси х и const y? Вдоль границы выделенной области окна?
     * аналогично В и С?
     * для какой области выбирается f?
     * как найти собственные числа? подбором u и v (точнее проходом по области окна)?
     * диапазон u и v?
     */

}

void MyPicture::Pyramid(int octave, double numLevel, double finishOctaveLevel)
{
    double k = pow(2.0, 1.0 / numLevel);
    double sigma;

    int picture_num = 0;

    if(octave != 0){
        for(int i = 0; i < octave; i++){
            sigma = 1;
            for(double finish = 0; finish <= finishOctaveLevel && finish <= numLevel; finish++){

                if((sigma == 1 && i == 0) || (sigma != 1)){
                    GaussianFilter(sigma);
                }

                sigma *= k;
                SavePictureInFile("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\1\\" + QString::number(picture_num) + ".bmp");
                picture_num++;

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
