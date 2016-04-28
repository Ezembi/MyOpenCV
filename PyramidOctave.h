#ifndef PYRAMIDOCTAVE_H
#define PYRAMIDOCTAVE_H
#include <PyramidLevel.h>


class PyramidOctave
{
private:
    std::vector<PyramidLevel> levels;   //уровни в данной октаве
    int n_;                             //номер октавы
public:
    PyramidOctave(const int numberOctave);
    int getNumberOctave() const;
    void addLevel(PyramidLevel& level);
    int nLevels() const;
    MyQImage getLevelImage(int i) const;
    int getLevelNumber(int i) const;
    std::vector<InterestingPoint> octaveDoG();                   //сделать DoG для данной октавы

};

#endif // PYRAMIDOCTAVE_H
