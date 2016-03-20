#include "PyramidOctave.h"

PyramidOctave::PyramidOctave(int numberOctave)
{
    n_ = numberOctave;
}

int PyramidOctave::getNumberOctave() const
{
    return n_;
}

std::vector<PyramidLevel> PyramidOctave::getLevels() const
{
    return levels;
}

void PyramidOctave::addLevel(PyramidLevel& level)
{
    levels.push_back(level);
}

int PyramidOctave::nLevels() const
{
    return levels.size();
}
