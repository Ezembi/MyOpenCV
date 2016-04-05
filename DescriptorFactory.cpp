#include "DescriptorFactory.h"

DescriptorFactory::DescriptorFactory(const MyQImage &image, int nIntrstingPoint)
{
    printf("Start: DescriptorFactory");
    L = image.sobel("All");
    Pfi.copy(image);

    MyQImage dx = image.sobel("X");
    MyQImage dy = image.sobel("Y");


    for(int x = 0; x < image.getWidth(); x++){
        for(int y = 0; y < image.getHeight(); y++){
            Pfi.setPixel(x,y, atan2(dy.getPixel(x,y), dx.getPixel(x,y)) * 180.0 / M_PI + 180.0);
        }
    }


    std::vector<InterestingPoint> points = image.ANMS(
                image.Harris(3,3,4,0.06),
                nIntrstingPoint
                );

    for(int i = 0; i < points.size(); i++){
        descriptors.push_back(getDescrIntrPoint(points[i]));
    }

    printf("DescriptorFactory OK (%d descriptors)\n", descriptors.size());

}

int DescriptorFactory::getDescriptorSize() const
{
    return descriptors.size();
}

double DescriptorFactory::getDescriptorsDistanse(const int dNumber, const Descriptor &descriptor) const
{
    return descriptors[dNumber].getDistanse(descriptor);
}

const Descriptor &DescriptorFactory::getDescriptor(int i)
{
    return descriptors[i];
}

Descriptor DescriptorFactory::getDescrIntrPoint(InterestingPoint point)
{
    Descriptor result(point);

    double binSize = 360.0 / nBin;                  // 1/8 от 360 - размер в градусах одной корзины
    int bin;                                        //номмер корзины
    int nGist = 0;                                  //номер гистограммы
    double distans;


    for(int i = -wGist; i < wGist; i++){
        for(int j = -hGist; j < hGist; j++){

            //узнаём в какую часть дескриптора добавлять
            if(i > 0 && j > 0)   nGist = 0;     //верхний левый
            if(i <= 0 && j > 0)  nGist = 1;     //верхний правый
            if(i > 0 && j <= 0)  nGist = 2;     //нижний левый
            if(i <= 0 && j <= 0) nGist = 3;     //нижний правый

            //узнаём номер корзины
            bin = Pfi.getPixel(point.x_ + i, point.y_ + j) / binSize;

            //узнаём расстояние от точки интереса
            distans = pointDistance((double)point.x_, (double)point.x_ + i, (double)point.y_, (double)point.y_ + j);
            if(distans == 0.0) distans = 1.0;       //чтоб не было деления на 0

            //Пропорционально распределяем L между 2 смежными корзинами (наша корзина и 2 смежные),
            //исходя из удаленности от точки интереса
            result.addGistValue(nGist, bin - 1, L.getPixel(point.x_ + i, point.y_ + j) / (3.0 * distans));
            result.addGistValue(nGist, bin, L.getPixel(point.x_ + i, point.y_ + j) / (3.0 * distans));
            result.addGistValue(nGist, bin + 1, L.getPixel(point.x_ + i, point.y_ + j) / (3.0 * distans));

        }
    }

    //нормализуем
    result.normalize();

    return result;
}
