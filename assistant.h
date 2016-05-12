#include <qmath.h>
#include <QtGlobal>
#include <GlobalNames.h>

#ifndef ASSISTANT
#define ASSISTANT

#include<vector>

//интересные точки
struct InterestingPoint
{
    int x_, y_, r_; //r - размер “глобальный” сигмы в масштабе исходного изображения
    double value_;
    double alpha_;
    int octave_;
    int level_;
    double sigma_;

    InterestingPoint()
    {
        x_ = 0;
        y_ = 0;
        r_ = 10;
        octave_ = -1;
        level_ = -1;
        value_ = 0.0;
        alpha_ = 0.0;
        sigma_ = 0.0;
    }

    InterestingPoint(int x, int y, double value)
    {
        x_ = x;
        y_ = y;
        r_ = 10;
        octave_ = -1;
        level_ = -1;
        value_ = value;
        alpha_ = 0.0;
        sigma_ = 0.0;
    }

    InterestingPoint(int x, int y, double value, double alpha)
    {
        x_ = x;
        y_ = y;
        r_ = 10;
        octave_ = -1;
        level_ = -1;
        value_ = value;
        alpha_ = alpha;
        sigma_ = 0.0;
    }

    InterestingPoint(int x, int y, int r, double value, double alpha)
    {
        x_ = x;
        y_ = y;
        r_ = r;
        octave_ = -1;
        level_ = -1;
        value_ = value;
        alpha_ = alpha;
        sigma_ = 0.0;
    }

    InterestingPoint(int x, int y, int r, int octave, int level, double value, double alpha)
    {
        x_ = x;
        y_ = y;
        r_ = r;
        octave_ = octave;
        level_ = level;
        value_ = value;
        alpha_ = alpha;
        sigma_ = 0.0;
    }

    InterestingPoint(int x, int y, int r, int octave, int level, double sigma, double value, double alpha)
    {
        x_ = x;
        y_ = y;
        r_ = r;
        octave_ = octave;
        level_ = level;
        value_ = value;
        alpha_ = alpha;
        sigma_ = sigma;
    }

    void print(){
        printf("x = %d, y = %d, r = %d, value = %lf, alpha = %0.10lf\n", x_, y_, r_, value_, alpha_);
    }

    double distance(InterestingPoint p){
        return sqrt((p.x_ - x_) * (p.x_ - x_) + (p.y_ - y_) * (p.y_ - y_));
    }

};

//обработка краевых эффектов (отражение коэф зеркально от 0 и от ширины / высоты)
int static reflect(int x, int maxX)
{
    if(x >= maxX){
        return maxX - (x - maxX) - 1;
    }

    if(x < 0){
        return -1 * x;
    }

    return x;
}

int static getIndex(int x, int y, int width, int height){
    return reflect(x, width) * height + reflect(y, height);
}

int static getIndex(int bin, int nBin, int nGist){
    //заворачиваем корзину
    if(bin >= nBin) bin = 0;
    if(bin < 0) bin = nBin - 1;

    return bin * nGistBin + nGist;
}


//расстояние между точкими
double static pointDistance(double x0, double x1, double y0, double y1){
    return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

//отобразить ядро всёртки
void static printKernel(double* kernel, int width, int height)
{
    for(int x = 0 ; x < width; x++) {
        for(int y = 0; y < height; y++){
            printf("%lf ",  kernel[x * height + y]);
        }
        printf("\n");
    }
}

//заполнения ядра Гауса
void static getGaussianKernel(double* kernel, int width, int height, double sigma_)
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
            numPow = -(x * x + y * y) / (2.0 * (sigma_ * sigma_));
            kernel[getIndex(qRound(xKernel), qRound(yKernel), width, height)] = pow(M_E, numPow) * (1.0 / (2.0 * M_PI * (sigma_ * sigma_)));
        }
    }
}

//Adaptive non-maximum suppression
std::vector<InterestingPoint> static ANMS(std::vector<InterestingPoint> points, int nPoint){
    printf("Start: ANMS\n");
    int radius = 0;
    bool isMax;

    do{
        radius++;

        for(int x = 0; x < points.size(); x++){
            for(int y = 0; y < points.size(); y++){
                isMax = true;
                if(x != y){
                    if( points[x].distance(points[y]) < radius &&
                            points[x].value_ < points[y].value_ ) {

                        isMax = false;
                    }

                    if(isMax){
                    } else {
                        points.erase(points.begin() + x);
                    }
                }
            }
        }
    }while(points.size() > nPoint);

    printf("ANMS OK\n");
    return points;
}



#endif // ASSISTANT

