#include "Pyramid.h"

Pyramid::Pyramid(MyQImage &resultImage, const double sigma0, const int octave, const double numLevel)
{
    printf("Start: Pyramid\n");

    MyQImage originalImage = resultImage;

    double k = pow(2.0, 1.0 / numLevel);

    double sigma = sigma0;                                  //сигма предыдущего уровня (глобальная)
    double levelSigma;                                      //сигма текущего уровня (глобальная)
    double deltaSigma;                                      //сигма для гауса (локальная)

    resultImage.gaussianFilter(originalImage, sigma);       //фильтруем гаусом до sigma 0


    for(int i = 0; i < octave; i++){

        sigma = sigma0;
        PyramidOctave octaves(i);

        for(double finish = 0; finish <= numLevel; finish++){

            levelSigma = sigma * k;
            deltaSigma = sqrt(levelSigma * levelSigma - sigma * sigma);

            resultImage.gaussianFilter(originalImage, deltaSigma);

            PyramidLevel level(originalImage, deltaSigma, levelSigma, finish);
            octaves.addLevel(level);

            originalImage.copy(resultImage);

            sigma = levelSigma;
        }

        addOctrve(octaves);

        originalImage.resizeAndCopy(resultImage, resultImage.getWidth() / 2, resultImage.getHeight() / 2);
        resultImage.copy(originalImage);
    }

    printf("Pyramid OK!\n");
}

std::vector<PyramidOctave> Pyramid::getOctaves() const
{
    return octaves;
}

void Pyramid::savePyramid(QString path) const
{
    for(int i = 0; i < nOctave(); i++){
        for(int j = 0; j < octaves[i].nLevels(); j++){
            octaves[i].levels[j].getImage().saveImage(
                        path +
                        "octave_" +
                        QString::number(octaves[i].getNumberOctave()) +
                        "_level_" +
                        QString::number(octaves[i].levels[j].getLevelNumber()) +
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
