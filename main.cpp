#include <QCoreApplication>
#include <MyPicture.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    {
        //MyPicture picture("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\m.jpg");
        //MyPicture picture("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\k.bmp");
        MyPicture picture("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Lenna.bmp");

        //picture.ConvertToNegative();
        //picture.HorizontalSwap();
        //picture.WerticalSwap();
        //picture.Blur();
        //picture.Sharpness();
        //picture.Sobel("All");   //X, or Y, or All
        //picture.GaussianFilter(5);
        //picture.ResizeImage(256,256);
        //picture.Pyramid(6,3,3);
        //picture.AddNoise(500);
        picture.Moravec(3,3,6,6,300,0.2);


        picture.SavePictureInFile("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\2.bmp");

    }

    printf("\n-=OK=-");
    exit(0);
    return a.exec();
}
