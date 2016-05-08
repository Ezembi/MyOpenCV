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
void Lab_6_7();
void Lab_8();
void draw2Image(const MyQImage &image1, const MyQImage &image2, const std::vector<Descriptor> &descriptors1, const std::vector<Descriptor> &descriptors2, QString saveFileName);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Lab_8();

    printf("\n-=OK=-");
    exit(0);
    return a.exec();
}

//Собель
void Lab_1(){
    MyQImage resultImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z2.jpg");
    MyQImage sobleX = resultImage.sobel("X");
    MyQImage sobleY = resultImage.sobel("Y");
    MyQImage sobleAll = resultImage.sobel("All");

    sobleX.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\sobleX.bmp");
    sobleY.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\sobleY.bmp");
    sobleAll.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\sobleAll.bmp");
    resultImage.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\3.bmp");
}

//Гаусс, Пирамиды
void Lab_2(){
    MyQImage resultImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z2.jpg");
    MyQImage gaus = resultImage.gaussianFilter(10);
    gaus.saveImage("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\gaus.bmp");
    //Pyramid pyramid(resultImage, 1.6, 5, 5);
    //pyramid.savePyramid("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\1\\");
}

//Операторы точек интереса
void Lab_3(){
    MyQImage image1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z2.jpg");
    MyQImage image2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z2.jpg");

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

    image.save("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\combo1.bmp");
}

//поворт
void Lab_5(){

    //грузим кртинки
//    MyQImage image1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z1.jpg");
//    MyQImage image2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\z2_130.jpg");
    MyQImage image1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\bild1.jpg");
    MyQImage image2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\bild2.jpg");

    //кол-во интересных точек
    int nPoint = 300;

    //получаем все дескрипторы для первого изображения
    std::vector<Descriptor> descriptors1;
    DescriptorFactory df1(image1);

    descriptors1 = df1.getDescriptors(ANMS(
                                          image1.Harris(3,3,4,0.06),
                                          nPoint
                                          )
                                      );

    //получаем все дескрипторы для второго изображения
    std::vector<Descriptor> descriptors2;
    DescriptorFactory df2(image2);

    descriptors2 = df2.getDescriptors(ANMS(
                                          image2.Harris(3,3,4,0.06),
                                          nPoint
                                          )
                                      );

    draw2Image(image1, image2, descriptors1, descriptors2, "D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\combo1.bmp");

}

//Блобы
void Lab_6_7(){
//    MyQImage resultImage1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\p1.png");
//    MyQImage resultImage2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\p2d2.bmp");
    MyQImage resultImage1("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\p1.jpg");
    MyQImage resultImage2("D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\p2.jpg");

    Pyramid pyramid1(resultImage1, 1.6, 7, 5);

    std::vector<InterestingPoint> blobs1;
    blobs1 = pyramid1.getBlobs();
    std::vector<Descriptor> descriptors1;

    for(int i = 0; i < pyramid1.nOctave(); i++)
    {
        for(int j = 0; j < pyramid1.nLevel(i); j++){
            std::vector<Descriptor> descriptors;
            DescriptorFactory df(pyramid1.getImage(i,j) ,i ,j);  //фабрика для уровня и октавы
            descriptors = df.getDescriptors(blobs1);            //дескрипторы уровня и октавы

            for(int k = 0; k < descriptors.size(); k++){
                //сохраняем дескрипторы для уровня и октавы
                descriptors1.push_back(descriptors[k]);
            }
        }
    }

    Pyramid pyramid2(resultImage2, 1.6, 7, 5);

    std::vector<InterestingPoint> blobs2;
    blobs2 = pyramid2.getBlobs();
    std::vector<Descriptor> descriptors2;

    for(int i = 0; i < pyramid2.nOctave(); i++)
    {
        for(int j = 0; j < pyramid2.nLevel(i); j++){
            std::vector<Descriptor> descriptors;
            DescriptorFactory df(pyramid2.getImage(i,j) ,i ,j);  //фабрика для уровня и октавы
            descriptors = df.getDescriptors(blobs2);            //дескрипторы уровня и октавы

            for(int k = 0; k < descriptors.size(); k++){
                //сохраняем дескрипторы для уровня и октавы
                descriptors2.push_back(descriptors[k]);
            }
        }
    }

    draw2Image(resultImage1, resultImage2, descriptors1, descriptors2, "D:\\Qt\\Qt5.5.1\\Projects\\PictureForMyOpenCv\\comboBlob0.bmp");
}

//панорама
void Lab_8(){
    //to be continued...
}

void draw2Image(const MyQImage &image1, const MyQImage &image2, const std::vector<Descriptor> &descriptors1, const std::vector<Descriptor> &descriptors2, QString saveFileName){
    //получаем загруженные картинки
    QImage qIamqe1 = image1.getQImage();
    QImage qIamqe2 = image2.getQImage();

    int x;
    int y;
    int r;
    double alpha;
    int octave;

    //создаём новое, пустое изображение по размеру = суммы ширины обоих картинок
    int height = (qIamqe1.height() > qIamqe2.height()) ? qIamqe1.height() : qIamqe2.height();
    QImage image( qIamqe1.width() + qIamqe2.width(), height, QImage::Format_RGB32 );

    //отрисовываем обе картинки параллельно друг-другу
    QPainter painter;
    painter.begin(&image);
    painter.drawImage(QPoint(0,0), qIamqe1);
    painter.drawImage(QPoint(qIamqe1.width(),0), qIamqe2);

    painter.setPen(Qt::red);

    //отрисовка max направлений
    for(int i = 0; i < descriptors1.size(); i++){

        x = descriptors1[i].getInterestingPoint().x_;
        y = descriptors1[i].getInterestingPoint().y_;
        r = descriptors1[i].getInterestingPoint().r_;
        alpha = descriptors1[i].getInterestingPoint().alpha_;
        octave = descriptors1[i].getInterestingPoint().octave_;
        if(octave != -1){
            x *= pow(2, octave);
            y *= pow(2, octave);
        }

        int newI = r / 2 * cos(alpha * M_PI / 180.0);
        int newJ = r / 2 * sin(alpha * M_PI / 180.0);
        painter.setPen(Qt::red);
        painter.drawLine(x, y, x + newI, y + newJ );
        painter.setPen(Qt::green);
        painter.drawRect(x - 1, y - 1, 2, 2);

        if(octave != -1){
        QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
        painter.setPen(pen);
        painter.drawEllipse(
                        x - r/2,
                        y - r/2,
                        r,
                        r
                    );
        }
    }

    for(int i = 0; i < descriptors2.size(); i++){

        x = descriptors2[i].getInterestingPoint().x_;
        y = descriptors2[i].getInterestingPoint().y_;
        r = descriptors2[i].getInterestingPoint().r_;
        alpha = descriptors2[i].getInterestingPoint().alpha_;
        octave = descriptors2[i].getInterestingPoint().octave_;
        if(octave != -1){
            x *= pow(2, octave);
            y *= pow(2, octave);
        }

        x += qIamqe1.width();

        int newI = r / 2 * cos(alpha * M_PI / 180.0);
        int newJ = r / 2 * sin(alpha * M_PI / 180.0);
        painter.setPen(Qt::red);
        painter.drawLine(x, y, x + newI, y + newJ );
        painter.setPen(Qt::green);
        painter.drawRect(x - 1, y - 1, 2, 2);

        if(octave != -1){
        QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
        painter.setPen(pen);
        painter.drawEllipse(
                        x - r/2,
                        y - r/2,
                        r,
                        r
                    );
        }
    }

    //поиск "похожих" дескрипторов
    std::vector<std::pair<InterestingPoint, InterestingPoint>> pairs = getPairs(descriptors1, descriptors2);

    for(int i = 0; i < pairs.size(); i++){

        //отрисовываем линии для "похожих" дескрипторов рандомными цветами
        QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
        painter.setPen(pen);

        int x0 = pairs[i].first.x_;
        int y0 = pairs[i].first.y_;
        int r0 = pairs[i].first.r_;
        int octave0 = pairs[i].first.octave_;

        int x1 = pairs[i].second.x_;
        int y1 = pairs[i].second.y_;
        int r1 = pairs[i].second.r_;
        int octave1 = pairs[i].second.octave_;

        if(octave0 != -1){
            x0 *= pow(2,octave0);
            y0 *= pow(2,octave0);
        }

        if(octave1 != -1){
            x1 *= pow(2,octave1);
            y1 *= pow(2,octave1);
        }

        x1 += qIamqe1.width();

        if(octave != -1){
            painter.drawEllipse(
                        x0 - r0/2,
                        y0 - r0/2,
                        r0,
                        r0
                        );
            painter.drawEllipse(
                        x1 - r1/2,
                        y1 - r1/2,
                        r1,
                        r1
                        );
        }
        painter.drawLine(QPoint(x0, y0), QPoint(x1, y1));
    }

    painter.end();

    image.save(saveFileName);
}
