#include "PyramidOctave.h"

PyramidOctave::PyramidOctave(int numberOctave)
{
    n_ = numberOctave;
}

int PyramidOctave::getNumberOctave() const
{
    return n_;
}

void PyramidOctave::addLevel(PyramidLevel& level)
{
    levels.push_back(level);
}

int PyramidOctave::nLevels() const
{
    return levels.size();
}

MyQImage PyramidOctave::getLevelImage(int i) const
{
    return levels[i].getImage();
}

int PyramidOctave::getLevelNumber(int i) const
{
    return levels[i].getLevelNumber();
}

std::vector<Blob> PyramidOctave::octaveDoG()
{
    for(int i = 0; i < levels.size() - 1; i++){
        levels[i].Subtraction(levels[i + 1]);
    }

    //удаляем верхушку
    levels.erase(levels.begin() + (levels.size() - 1));

    std::vector<Blob> blobs;

    for(int i = 1; i < levels.size() - 1; i++){
        for(int x = 1; x < levels[i].getWidth()-1; x++){
            for(int y = 1; y < levels[i].getHeight()-1; y++){
                double pixel = levels[i].getPixel(x,y);
                if(
                    levels[i - 1].isExtremum(pixel, x - 1, y - 1, x + 1, y + 1, false) &&
                    levels[  i  ].isExtremum(pixel, x - 1, y - 1, x + 1, y + 1, true) &&
                    levels[i + 1].isExtremum(pixel, x - 1, y - 1, x + 1, y + 1, false)
                   ){
                    Blob blob((x * (n_ + 1)) - (n_ + 1) / 2,(y * (n_ + 1)) - (n_ + 1) / 2,levels[i].getGlobalSigma() * sqrt(2.0) * (n_ + 1),pixel);
                    blobs.push_back(blob);
                }
            }
        }
    }

    printf("octaveDoG = %d\n" , blobs.size());
    return blobs;
}
