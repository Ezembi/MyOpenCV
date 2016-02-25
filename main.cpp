#include <QCoreApplication>
#include <MyPicture.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MyPicture picture("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Lenna.bmp");

    //picture.ConvertToNegative();
    //picture.HorizontalSwap();
    //picture.WerticalSwap();
    //picture.Blur();
    //picture.Sharpness();
    picture.Sobel("All");   //X, or Y, or All




    picture.SavePictureInFile("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\2.jpg");

    printf("%d x %d\n", picture.Width(), picture.Height());

    printf("\n-=OK=-");
    exit(0);
    return a.exec();
}
