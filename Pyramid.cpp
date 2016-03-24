#include "Pyramid.h"

Pyramid::Pyramid(const MyQImage &Image, const double sigma0, const int octave, const double numLevel)
{
    printf("Start: Pyramid\n");

    MyQImage levelImage = Image;                            //картинка для свёртки и записи в level

    MyQImage tmpImage = levelImage;                         //картинка для хранения промежуточных данных

    double k = pow(2.0, 1.0 / numLevel);

    double sigma = sigma0;                                  //сигма предыдущего уровня (глобальная)
    double levelSigma;                                      //сигма текущего уровня (глобальная)
    double deltaSigma;                                      //сигма для гауса (локальная)
    deltaSigma = sqrt(sigma * sigma - 0.5 * 0.5);

    tmpImage = Image.gaussianFilter(deltaSigma);            //фильтруем гаусом до sigma 0

    for(int i = 0; i < octave; i++){

        sigma = sigma0;
        PyramidOctave octaves(i);

        for(double finish = 0; finish <= numLevel; finish++){

            levelSigma = sigma * k;
            deltaSigma = sqrt(levelSigma * levelSigma - sigma * sigma);

            levelImage = tmpImage.gaussianFilter(deltaSigma);

            PyramidLevel level(levelImage, deltaSigma, levelSigma, finish);
            octaves.addLevel(level);

            tmpImage.copy(levelImage);

            sigma = levelSigma;
        }

        addOctrve(octaves);

        tmpImage.resizeAndCopy(levelImage, levelImage.getWidth() / 2, levelImage.getHeight() / 2);
        levelImage.copy(tmpImage);
    }

    printf("Pyramid OK!\n");
}

void Pyramid::savePyramid(QString path) const
{
    for(int i = 0; i < nOctave(); i++){
        for(int j = 0; j < octaves[i].nLevels(); j++){
            octaves[i].getLevelImage(j).saveImage(
                        path +
                        "octave_" +
                        QString::number(octaves[i].getNumberOctave()) +
                        "_level_" +
                        QString::number(octaves[i].getLevelNumber(j)) +
                        ".bmp"
                        );
        }
    }
}

void Pyramid::addOctrve(PyramidOctave &octave)
{
    octaves.push_back(octave);
}

int Pyramid::nOctave() const
{
    return octaves.size();
}

MyQImage Pyramid::getImage(int nOctav, int nLevel) const
{
    return octaves[nOctav].getLevelImage(nLevel);
}
