#include "PyramidLevel.h"


PyramidLevel::PyramidLevel(const MyQImage &image, double localSigma, double globalSigma, int levelNumber)
{
    image_.copy(image);
    localSigma_ = localSigma;
    globalSigma_ = globalSigma;
    n_ = levelNumber;
}

MyQImage PyramidLevel::getImage() const
{
    return image_;
}

double PyramidLevel::getLocalSigma() const
{
    return localSigma_;
}

double PyramidLevel::getGlobalSigma() const
{
    return globalSigma_;
}

int PyramidLevel::getLevelNumber() const
{
    return n_;
}
