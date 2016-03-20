#ifndef PYRAMIDOCTAVE_H
#define PYRAMIDOCTAVE_H
#include <PyramidLevel.h>


class PyramidOctave
{
public:
    std::vector<PyramidLevel> levels;   //уровни в данной октаве
private:
    int n_;                             //номер уровня
public:
    PyramidOctave(const int numberOctave);
    int getNumberOctave() const;
    std::vector<PyramidLevel> getLevels() const;
    void addLevel(PyramidLevel& level);
    int nLevels() const;


};

#endif // PYRAMIDOCTAVE_H
