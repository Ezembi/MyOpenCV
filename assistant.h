#include <qmath.h>
#include <QtGlobal>
#ifndef ASSISTANT
#define ASSISTANT

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

//Adaptive non-maximum suppression
int static ANMS(const double* arrayInterestingPoint, int width, int height, int nPoint, double T)
{
    int nLocal;
    int radius = 0;
    bool isMax;

    do{
        radius++;
        nLocal = 0;

        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++){
                isMax = true;
                for(int px = -radius; px < radius; px++){
                    for(int py = -radius; py < radius; py++){
                        if(arrayInterestingPoint[getIndex(x, y, width, height)] < arrayInterestingPoint[getIndex(reflect(x + px, width), reflect(y + py, width), width, height)]){
                            isMax = false;
                        }
                    }
                }

                if(isMax && arrayInterestingPoint[x * height + y] > T){
                    nLocal++;
                }
            }
        }
    }while(nLocal > nPoint);

    return radius;
}

//заполнения ядра Гауса
void static getGaussianKernel(double* kernel, int width, int height, double sigma)
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
            numPow = -(x * x + y * y) / (2.0 * (sigma * sigma));
            kernel[getIndex(qRound(xKernel), qRound(yKernel), width, height)] = pow(M_E, numPow) * (1.0 / (2.0 * M_PI * (sigma * sigma)));
        }
    }
}

#endif // ASSISTANT

