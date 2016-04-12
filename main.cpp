#include <QCoreApplication>
#include<MyQImage.h>
#include<Pyramid.h>
#include <DescriptorFactory.h>
#include<iostream>

void Lab_1();
void Lab_2();
void Lab_3();
void Lab_4();
void Lab_5();
void Lab_6();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Lab_6();

    printf("\n-=OK=-");
    exit(0);
    return a.exec();
}

//Собель
void Lab_1(){
    MyQImage resultImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Lenna.bmp");
    MyQImage originalImage = resultImage;
    originalImage = resultImage.blur();
    originalImage = resultImage.sharpness();
    originalImage = resultImage.sobel("All");
    resultImage.verticalSwap();
    resultImage.horizontalSwap();
    originalImage.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\2.bmp");
    resultImage.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\3.bmp");
}

//Гаусс, Пирамиды
void Lab_2(){
    MyQImage resultImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Lenna.bmp");

    Pyramid pyramid(resultImage, 1.6, 5, 5);
    pyramid.savePyramid("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\1\\");
}

//Операторы точек интереса
void Lab_3(){
    MyQImage image1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Lenna.bmp");
    MyQImage image2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Lenna.bmp");

    MyQImage originalImage1 = image1;
    originalImage1.saveImage(
                "D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Harris.bmp",
                ANMS(
                    image1.Harris(3,3,4,0.06),
                    300
                    )
                );
    MyQImage originalImage2 = image2;
    originalImage2.saveImage(
                "D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\Moravec.bmp",
                ANMS(
                    image2.Moravec(5,5,3,3,0.2),
                    300
                    )
                );
}

//Дескрипторы окрестности интересных точек
void Lab_4(){

    //грузим кртинки
    MyQImage image1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z1.jpg");
    MyQImage image2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z2.jpg");
//    MyQImage image1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\a1.jpg");
//    MyQImage image2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\a2.jpg");

    //кол-во интересных точек
    int nPoint = 300;

    //получаем все дескрипторы для первого изображения
    std::vector<Descriptor> descriptors1;

    std::vector<InterestingPoint> points1 = ANMS(
                image1.Harris(3,3,4,0.06),
                nPoint
                );

    DescriptorFactory df1(image1);
    for(int i = 0; i < points1.size(); i++){
        descriptors1.push_back(df1.getDescrIntrPoint(points1[i]));
    }

    //получаем все дескрипторы для второго изображения
    std::vector<Descriptor> descriptors2;

    std::vector<InterestingPoint> points2 = ANMS(
                image2.Harris(3,3,4,0.06),
                nPoint
                );

    DescriptorFactory df2(image2);
    for(int i = 0; i < points2.size(); i++){
        descriptors2.push_back(df2.getDescrIntrPoint(points2[i]));
    }

    //получаем загруженные картинки
    QImage qIamqe1 = image1.getQImage();
    QImage qIamqe2 = image2.getQImage();

    //создаём новое, пустое изображение по размеру = суммы ширины обоих картинок
    int height = (qIamqe1.height() > qIamqe2.height()) ? qIamqe1.height() : qIamqe2.height();
    QImage image( qIamqe1.width() + qIamqe2.width(), height, QImage::Format_RGB32 );

    //отрисовываем обе картинки параллельно друг-другу
    QPainter painter;
    painter.begin(&image);
    painter.drawImage(QPoint(0,0), qIamqe1);
    painter.drawImage(QPoint(qIamqe1.width(),0), qIamqe2);

    //поиск "похожих" дескрипторов
    for(int i = 0; i < descriptors1.size(); i++){

        double minDist1 = descriptors1[i].getDistance(descriptors2[0]);
        int minJ1 = 0;
        double minDist2 = descriptors1[i].getDistance(descriptors2[1]);
        int minJ2 = 1;

        if(minDist1 > minDist2){
            std::swap(minDist1, minDist2);
            std::swap(minJ1, minJ2);
        }

        for(int j = 2; j < descriptors2.size(); j++){

            double dist = descriptors1[i].getDistance(descriptors2[j]);

            if(dist < minDist2){
                minDist2 = dist;
                minJ2 = j;
            }

            if(dist < minDist1){
                std::swap(minDist1, minDist2);
                std::swap(minJ1, minJ2);
            }
        }

        if(minDist1 / minDist2 < 0.8){
            //отрисовываем линии для "похожих" дескрипторов рандомными цветами
            QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
            painter.setPen(pen);
            painter.drawLine(
                        QPoint(
                            descriptors1[i].getInterestingPoint().x_,
                            descriptors1[i].getInterestingPoint().y_
                            ),
                        QPoint(
                            descriptors2[minJ1].getInterestingPoint().x_ + qIamqe1.width(),
                            descriptors2[minJ1].getInterestingPoint().y_
                            )
                        );
        }

    }

    painter.end();

    image.save("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\combo.bmp");
}

void Lab_5(){

    //грузим кртинки
    MyQImage image1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z1.jpg");
    MyQImage image2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z2_130.jpg");
//    MyQImage image1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\a1.jpg");
//    MyQImage image2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\a2.jpg");

    //кол-во интересных точек
    int nPoint = 300;

    //получаем все дескрипторы для первого изображения
    std::vector<Descriptor> descriptors1;

    std::vector<InterestingPoint> points1 = ANMS(
                image1.Harris(3,3,4,0.06),
                nPoint
                );

    DescriptorFactory df1(image1);
    for(int i = 0; i < points1.size(); i++){
        descriptors1.push_back(df1.getNormalDescrIntrPoint(points1[i]));
    }

    //получаем все дескрипторы для второго изображения
    std::vector<Descriptor> descriptors2;

    std::vector<InterestingPoint> points2 = ANMS(
                image2.Harris(3,3,4,0.06),
                nPoint
                );

    DescriptorFactory df2(image2);
    for(int i = 0; i < points2.size(); i++){
        descriptors2.push_back(df2.getNormalDescrIntrPoint(points2[i]));
    }

    //получаем загруженные картинки
    QImage qIamqe1 = image1.getQImage();
    QImage qIamqe2 = image2.getQImage();

    //создаём новое, пустое изображение по размеру = суммы ширины обоих картинок
    int height = (qIamqe1.height() > qIamqe2.height()) ? qIamqe1.height() : qIamqe2.height();
    QImage image( qIamqe1.width() + qIamqe2.width(), height, QImage::Format_RGB32 );

    //отрисовываем обе картинки параллельно друг-другу
    QPainter painter;
    painter.begin(&image);
    painter.drawImage(QPoint(0,0), qIamqe1);
    painter.drawImage(QPoint(qIamqe1.width(),0), qIamqe2);

    painter.setPen(Qt::red);

    for(int i = 0; i < points1.size(); i++){
        painter.drawRect(points1[i].x_ - 1, points1[i].y_ - 1, 2, 2);
    }
    for(int i = 0; i < points2.size(); i++){
        painter.drawRect(qIamqe1.width() + points2[i].x_ - 1, points2[i].y_ - 1, 2, 2);
    }

    //поиск "похожих" дескрипторов
    for(int i = 0; i < descriptors1.size(); i++){

        double minDist1 = descriptors1[i].getDistance(descriptors2[0]);
        int minJ1 = 0;
        double minDist2 = descriptors1[i].getDistance(descriptors2[1]);
        int minJ2 = 1;

        if(minDist1 > minDist2){
            std::swap(minDist1, minDist2);
            std::swap(minJ1, minJ2);
        }

        for(int j = 2; j < descriptors2.size(); j++){

            double dist = descriptors1[i].getDistance(descriptors2[j]);

            if(dist < minDist2){
                minDist2 = dist;
                minJ2 = j;
            }

            if(dist < minDist1){
                std::swap(minDist1, minDist2);
                std::swap(minJ1, minJ2);
            }
        }

        if(minDist1 / minDist2 < 0.8){
            //отрисовываем линии для "похожих" дескрипторов рандомными цветами
            QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
            painter.setPen(pen);
            painter.drawLine(
                        QPoint(
                            descriptors1[i].getInterestingPoint().x_,
                            descriptors1[i].getInterestingPoint().y_
                            ),
                        QPoint(
                            descriptors2[minJ1].getInterestingPoint().x_ + qIamqe1.width(),
                            descriptors2[minJ1].getInterestingPoint().y_
                            )
                        );
        }

    }

    painter.end();

    image.save("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\combo.bmp");
}

//Блобы
void Lab_6(){
    MyQImage resultImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\b.jpg");

    Pyramid pyramid(resultImage, 1.6, 5, 5);
    pyramid.savePyramid("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\1\\");

    std::vector<std::vector<Blob>> blobs;
    blobs = pyramid.DoG();

    pyramid.savePyramid("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\2\\");

    QImage qIamqe1 = resultImage.getQImage();
    QImage image(qIamqe1.width(), qIamqe1.height(), QImage::Format_RGB32 );

    QPainter painter;
    painter.begin(&image);
    painter.drawImage(QPoint(0,0), qIamqe1);

//    painter.drawEllipse();
    for(int i = 0; i < blobs.size(); i++){
        for(int j = 0; j < blobs[i].size(); j++){
            QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
            painter.setPen(pen);
            painter.drawEllipse(blobs[i][j].x_, blobs[i][j].y_, blobs[i][j].r_, blobs[i][j].r_);
        }
    }

    painter.end();

    image.save("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\blobs.bmp");
}
