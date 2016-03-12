#include <QCoreApplication>
#include <MyPicture.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    {
        MyPicture picture("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\m.jpg");

        //picture.ConvertToNegative();
        //picture.HorizontalSwap();
        //picture.WerticalSwap();
        //picture.Blur();
        //picture.Sharpness();
        //picture.Sobel("All");   //X, or Y, or All
        //picture.GaussianFilter(1);
        //picture.ResizeImage(256,256);
        picture.Pyramid(1,5,5);

        picture.SavePictureInFile("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\2.jpg");

    }

    printf("\n-=OK=-");
    exit(0);
    return a.exec();
}
