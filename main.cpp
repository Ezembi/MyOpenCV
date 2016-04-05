#include <QCoreApplication>
#include<MyQImage.h>
#include<Pyramid.h>
#include <DescriptorFactory.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //    MyQImage resultImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\m.jpg");
    //    MyQImage resultImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Lenna.bmp");
    MyQImage image1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z1.jpg");
    MyQImage image2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z2.jpg");

    //    MyQImage originalImage = resultImage;

    int nPoint = 100;
    DescriptorFactory df1(image1, nPoint);
    DescriptorFactory df2(image2, nPoint);

    QImage qIamqe1 = image1.getQImage();
    QImage qIamqe2 = image2.getQImage();

    QImage image( qIamqe1.width() + qIamqe2.width(), qIamqe1.height(), QImage::Format_RGB32 );

    QPainter painter;
    painter.begin(&image);
    painter.drawImage(QPoint(0,0), qIamqe1);
    painter.drawImage(QPoint(qIamqe2.width(),0), qIamqe2);

    for(int i = 0; i < df1.getDescriptorSize(); i++){

        double min = df1.getDescriptorsDistanse(i, df2.getDescriptor(0));
        int minJ = 0;

        for(int j = 1; j < df2.getDescriptorSize(); j++){

            double dist = df1.getDescriptorsDistanse(i, df2.getDescriptor(j));

            if(dist < min){
                if(dist / min < 0.8){
                    min = dist;
                    minJ = j;
                }
            }
        }

        QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
        painter.setPen(pen);
        painter.drawLine(
                    QPoint(
                        df1.getDescriptor(i).getInterestingPoint().x_,
                        df1.getDescriptor(i).getInterestingPoint().y_
                        ),
                    QPoint(
                        df2.getDescriptor(minJ).getInterestingPoint().x_ + qIamqe2.width(),
                        df2.getDescriptor(minJ).getInterestingPoint().y_
                        )
                    );

    }

    painter.end();

    image.save("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\combo.bmp");



    //    Pyramid pyramid(resultImage, 1.6, 5, 4);
    //    pyramid.savePyramid("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\1\\");

    //    originalImage = resultImage.gaussianFilter(5);
    //    originalImage = resultImage.blur();
    //    originalImage = resultImage.sharpness();
    //    originalImage = resultImage.sobel("All");

    //    MyQImage originalImage1 = image1;
    //    originalImage1.saveImage(
    //                "D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Harris1.bmp",
    //                image1.ANMS(
    //                    image1.Harris(3,3,4,0.06),
    //                    300
    //                    )
    //                );
    //    MyQImage originalImage2 = image2;
    //    originalImage2.saveImage(
    //                "D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Harris2.bmp",
    //                image2.ANMS(
    //                    image2.Harris(3,3,4,0.06),
    //                    300
    //                    )
    //                );

    //    originalImage = resultImage.Harris(3,3,300,4,0.06);
    //    resultImage.verticalSwap();
    //    resultImage.horizontalSwap();

    //    MyQImage littleImage(originalImage, 512, 256);

    //    littleImage.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\li.bmp");
    //    originalImage.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\2.bmp");
    //    resultImage.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\3.bmp");
    printf("\n-=OK=-");
    exit(0);
    return a.exec();
}
