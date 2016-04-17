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

int PyramidLevel::getWidth() const
{
    return image_.getWidth();
}

int PyramidLevel::getHeight() const
{
    return image_.getHeight();
}

double PyramidLevel::getPixel(int x, int y)
{
    return image_.getPixel(x,y);
}

void PyramidLevel::Subtraction(const PyramidLevel &level)
{
    for(int x = 0; x < image_.getWidth(); x++){
        for(int y = 0; y < image_.getHeight(); y++){
            double im1 = image_.getPixel(x,y);
            double im2 = level.image_.getPixel(x,y);
            double raz = im2 - im1;
            image_.setPixel(x,y, raz);
        }
    }
}
