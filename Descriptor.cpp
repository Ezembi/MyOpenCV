#include "Descriptor.h"

Descriptor::Descriptor(InterestingPoint point)
{
    point_.x_ = point.x_;
    point_.y_ = point.y_;
    point_.value_ = point.value_;

    for(int i = 0; i < nGistBin; i++){
        for(int j = 0; j < nBin; j++){
            setGistValue(i,j, 0.0);
        }
    }

}

void Descriptor::setGistValue(int nGist, int bin, double value)
{
    //заворачиваем корзину
    if(bin >= nBin) bin = 0;
    if(bin < 0) bin = nBin - 1;

    int index = bin + nGist * nBin;
    gist_[index] = value;
}

void Descriptor::addGistValue(int nGist, int bin, double addValue)
{
    setGistValue(nGist, bin, getGistValue(nGist, bin) + addValue);
}

double Descriptor::getGistValue(int nGist, int bin) const
{
    int index = bin + nGist * nBin;
    return gist_[index];
}

InterestingPoint Descriptor::getInterestingPoint() const
{
    return point_;
}

double Descriptor::getDistanse(const Descriptor &descriptor) const
{
    double result = 0;
    for(int i = 0; i < nGistBin; i++){
        for(int j = 0; j < nBin; j++){
            result += (descriptor.getGistValue(i,j) - getGistValue(i,j)) * (descriptor.getGistValue(i,j) - getGistValue(i,j));
        }
    }
    return sqrt(result);
}

void Descriptor::normalize()
{
    double normNum = detNormalNumber();

    //нормализуем
    for(int i = 0; i < nGistBin; i++){
        for(int j = 0; j < nBin; j++){

            setGistValue(i,j, getGistValue(i,j) / normNum);

            //удаляем блики
            if(getGistValue(i,j) > 0.2){
                setGistValue(i,j,0.2);
            }
        }
    }

    //повторно нормализуем
    for(int i = 0; i < nGistBin; i++){
        for(int j = 0; j < nBin; j++){
            setGistValue(i,j, getGistValue(i,j) / normNum);
        }
    }

}

double Descriptor::detNormalNumber()
{
    double result = 0;
    for(int i = 0; i < nGistBin; i++){
        for(int j = 0; j < nBin; j++){
            result += getGistValue(i,j) * getGistValue(i,j);
        }
    }
    return sqrt(result);
}
