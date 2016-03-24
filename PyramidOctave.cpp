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
