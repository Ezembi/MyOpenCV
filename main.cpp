#include <QCoreApplication>
#include<MyQImage.h>
#include<Pyramid.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    {


        //MyQImage resultImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\m.jpg");
        //MyQImage resultImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\k.bmp");
        MyQImage resultImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Lenna.bmp");

        MyQImage originalImage = resultImage;

        Pyramid pyramid(resultImage, 1.6, 5, 4);
        pyramid.savePyramid("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\1\\");

        //originalImage = resultImage.gaussianFilter(5);
        //originalImage = resultImage.blur();
        //originalImage = resultImage.sharpness();
        //originalImage = resultImage.sobel("All");
        //originalImage = resultImage.Moravec(3,3,6,6,300,0.2);
        //originalImage = resultImage.Harris(3,3,300,4,0.06);
        //resultImage.verticalSwap();
        //resultImage.horizontalSwap();

        //MyQImage littleImage(originalImage, 512, 256);

        //littleImage.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\li.bmp");
        //originalImage.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\2.bmp");
        //resultImage.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\3.bmp");

    }

    printf("\n-=OK=-");
    exit(0);
    return a.exec();
}
