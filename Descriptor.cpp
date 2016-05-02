#include "Descriptor.h"

Descriptor::Descriptor(InterestingPoint point)
{
    point_.x_ = point.x_;
    point_.y_ = point.y_;
    point_.value_ = point.value_;
    point_.alpha_ = point.alpha_;

    for(int i = 0; i < nGistBin; i++){
        for(int j = 0; j < nBin; j++){
            setGistValue(i,j, 0.0);
        }
    }

}

void Descriptor::setGistValue(int nGist, int bin, double value)
{
    gist_[getIndex(bin, nBin, nGist)] = value;
}

void Descriptor::addGistValue(int nGist, int bin, double addValue)
{
    setGistValue(nGist, bin, getGistValue(nGist, bin) + addValue);
}

double Descriptor::getGistValue(int nGist, int bin) const
{
    return gist_[getIndex(bin, nBin, nGist)];
}

const InterestingPoint &Descriptor::getInterestingPoint() const
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
    if(result == 0) result = 0.0000000001;
    return sqrt(result);
}

void Descriptor::print()
{
    for(int i = 0; i < nGistBin; i++){
         printf("\n");
        for(int j = 0; j < nBin; j++){
             printf("%lf ",getGistValue(i,j));
        }
    }
    printf("\n\n");
}




