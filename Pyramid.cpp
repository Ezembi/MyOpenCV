#include "Pyramid.h"

Pyramid::Pyramid(const MyQImage &Image, const double sigma0, const int octave, const double numLevel)
{
    printf("Start: Pyramid\n");

    MyQImage levelImage = Image;                            //картинка для свёртки и записи в level

    MyQImage tmpImage = levelImage;                         //картинка для хранения промежуточных данных

    double k = pow(2.0, 1.0 / numLevel);

    double sigma = sigma0;                                  //сигма предыдущего уровня (глобальная)
    double deltaSigma;                                      //сигма для гауса (локальная)
    deltaSigma = sqrt(sigma * sigma - 0.8 * 0.8);

    tmpImage = Image.gaussianFilter(deltaSigma);            //фильтруем гаусом до sigma 0
    //tmpImage.normalizeTo0_1();

    for(int i = 0; i < octave; i++){

        sigma = sigma0;
        PyramidOctave octaves(i);

        for(double level = 0; level <= numLevel; level++){

            //double levelSigma = sigma0 * pow(k, level);     //сигма текущего уровня (глобальная)
            double levelSigma = sigma * k;

            deltaSigma = sqrt(levelSigma * levelSigma - sigma * sigma);

            levelImage = tmpImage.gaussianFilter(deltaSigma);
            printf("g = %lf\n", levelSigma);

            //levelImage.normalizeTo0_1();    //нормируем,
            //иначе: (level - 1) = ~0.X...; level = ~0.0000X...; (level + 1) = ~ 0.00000000X...

            PyramidLevel Level(levelImage, deltaSigma, levelSigma, level);
            octaves.addLevel(Level);

//            tmpImage.copy(levelImage);
            tmpImage = levelImage;

            sigma = levelSigma;
        }

        addOctrve(octaves);

        tmpImage.resizeAndCopy(levelImage, qRound((double)levelImage.getWidth() / 2.0), qRound((double)levelImage.getHeight() / 2.0));
        levelImage = tmpImage;
//        levelImage.copy(tmpImage);
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

std::vector<std::vector<Blob>> Pyramid::getBlobs()
{
    printf("Start: DoG\n");

    std::vector<std::vector<Blob>> blobs;

    for(int i = 0; i < octaves.size(); i++){
        blobs.push_back(octaves[i].octaveDoG());
    }
    printf("DoG OK\n");
    return blobs;
}
