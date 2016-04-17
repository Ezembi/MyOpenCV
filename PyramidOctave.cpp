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
                bool max = true;
                bool min = true;

                //поиск экстремума в 26 точках
                for(int ii = i - 1; ii <= i + 1; ii++){
                    for(int xi = x - 1; xi <= x + 1; xi++){
                        for(int yi = y - 1; yi <= y + 1; yi++){
                            if(xi == x && yi == y){
                                //пропуск pixel'я, который считаем экстремумом
                            } else {
                                if(pixel <= levels[ii].getPixel(xi,yi)){
                                    max = false;
                                }

                                if(pixel >= levels[ii].getPixel(xi,yi)){
                                    min = false;
                                }
                            }
                        }
                    }
                }

                if(max){
                    Blob blob(x * pow(2,n_),y * pow(2,n_),sqrt(2.0) * pow(2,n_),pixel);
                    blobs.push_back(blob);
                }
                if(min){
                    Blob blob(x * pow(2,n_),y * pow(2,n_),sqrt(2.0) * pow(2,n_),pixel);
                    blobs.push_back(blob);
                }
            }
        }
    }

    printf("octaveDoG = %d\n" , blobs.size());
    return blobs;
}
