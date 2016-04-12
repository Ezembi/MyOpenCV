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
    //заворачиваем корзину
    if(bin >= nBin) bin = 0;
    if(bin < 0) bin = nBin - 1;

    int index = bin + nGist * nBin;
    return gist_[index];
}

InterestingPoint Descriptor::getInterestingPoint() const
{
    return point_;
}

double Descriptor::getDistance(const Descriptor &descriptor) const
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
    double normNum = getVectorSize();

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

    normNum = getVectorSize();

    //повторно нормализуем
    for(int i = 0; i < nGistBin; i++){
        for(int j = 0; j < nBin; j++){
            setGistValue(i,j, getGistValue(i,j) / normNum);
        }
    }

}

double Descriptor::getVectorSize()
{
    double result = 0;
    for(int i = 0; i < nGistBin; i++){
        for(int j = 0; j < nBin; j++){
            result += getGistValue(i,j) * getGistValue(i,j);
        }
    }
    if(result == 0) result = 1;
    return sqrt(result);
}

void Descriptor::biggestInTop()
{
    for(int i = 0; i < nGistBin; i++){

        double max = getGistValue(i,0);
        int maxJ = 0;

        for(int j = 0; j < nBin; j++){
            if(max < getGistValue(i,j)){
                max = getGistValue(i,j);
                maxJ = j;
            }
        }

        if(maxJ != 0){
            for(int j = maxJ; j < nBin; j++){
                for(int k = j-1; k >= j - maxJ; k--){
                    double tmp = getGistValue(i,k+1);
                    setGistValue(i,k+1, getGistValue(i,k));
                    setGistValue(i,k, tmp);
                }
            }
        }
    }
}
