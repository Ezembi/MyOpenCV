#include <MyQImage.h>
#include <assistant.h>
#ifndef LAB_2
#define LAB_2

/**
 * @brief Pyramid                       //пирамидальное масштабирование
 * @param octave                        //кол-во октав (во скоько раз уменьшить изображение)
 * @param numLevel                      //кол-во уровней в октаве
 * @param finishOctaveLevel             //на каком уровне в октаве остановиться
 */
std::vector<MyQImage> Pyramid(int octave, double numLevel, double finishOctaveLevel)
{

    double k = pow(2.0, 1.0 / numLevel);
    double sigma;

    int picture_num = 0;

    if(octave != 0){
        for(int i = 0; i < octave; i++){
            sigma = 1;
            for(double finish = 0; finish <= finishOctaveLevel && finish <= numLevel; finish++){

                if((sigma == 1 && i == 0) || (sigma != 1)){
                    //gaussianFilter(sigma);
                }

                sigma *= k;
                //SavePictureInFile("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\1\\" + QString::number(picture_num) + ".bmp");
                picture_num++;

            }
            //ResizeImage(Width() / 2, Height() / 2);
        }
    }
}

#endif // LAB_2

