#include "DescriptorFactory.h"

DescriptorFactory::DescriptorFactory(const MyQImage &image)
{
    printf("Start: DescriptorFactory\n");
    L = image.sobel("All");
    Pfi.copy(image);

    MyQImage dx = image.sobel("X");
    MyQImage dy = image.sobel("Y");


    for(int x = 0; x < image.getWidth(); x++){
        for(int y = 0; y < image.getHeight(); y++){
            Pfi.setPixel(x,y, atan2(dy.getPixel(x,y), dx.getPixel(x,y)) * 180.0 / M_PI + 180.0);
        }
    }

    printf("DescriptorFactory OK\n");

}

Descriptor DescriptorFactory::getDescrIntrPoint(const InterestingPoint point)
{
    Descriptor result(point);

    double binSize = 360.0 / nBin;                  //1/8 от 360 - размер в градусах одной корзины
    int bin;                                        //номер корзины
    int relatedBin;                                 //номер смежной корзины
    int nGist = 0;                                  //номер гистограммы
    double disToCurCen;                             //дистанция до текущего центра
    double disToRelCen;                             //дистанция до смежного центра

    for(int i = -wGist; i < wGist; i++){
        for(int j = -hGist; j < hGist; j++){

            double localPfi = Pfi.getPixel(point.x_ + i, point.y_ + j);
            double localBinCenter;

            //узнаём в какую часть дескриптора добавлять
            if(i > 0 && j > 0)   nGist = 0;     //верхний левый
            if(i <= 0 && j > 0)  nGist = 1;     //верхний правый
            if(i > 0 && j <= 0)  nGist = 2;     //нижний левый
            if(i <= 0 && j <= 0) nGist = 3;     //нижний правый

            //            nGist = ( abs(i + wGist + 1) / nGistBin) * (hGist / 2) + (abs(j + hGist + 1) / nGistBin);


            //узнаём номер текущей корзины
            bin = localPfi / binSize;

            //смежная корзина = следующая, для localPfi >= localBinCenter
            relatedBin = bin + 1;

            //узнаём центр текущей корзины
            localBinCenter = (double)bin * binSize + binSize / 2.0;

            //узнаём смежную корзину, для localPfi < localBinCenter (предидущая корзина)
            if(localPfi < localBinCenter) relatedBin = bin - 1;

            //узнаём расстояния
            disToCurCen = abs(localBinCenter - localPfi);
            disToRelCen = binSize - disToCurCen;

            //Обратнопропорционально распределяем Pfi между центрами
            //2-х смежных корзин, исходя из расстояния
            result.addGistValue(nGist, bin,        L.getPixel(point.x_ + i, point.y_ + j) * (1 - disToCurCen / binSize));
            result.addGistValue(nGist, relatedBin, L.getPixel(point.x_ + i, point.y_ + j) * (1 - disToRelCen / binSize));

        }
    }

    //нормализуем
    result.normalize();

    return result;
}

Descriptor DescriptorFactory::getNormalDescrIntrPoint(const InterestingPoint point)
{
    Descriptor result(point);

    double binSize = 360.0 / nBin;                  //1/8 от 360 - размер в градусах одной корзины
    int bin;                                        //номер корзины
    int relatedBin;                                 //номер смежной корзины
    int nGist = 0;                                  //номер гистограммы
    double disToCurCen;                             //дистанция до текущего центра
    double disToRelCen;                             //дистанция до смежного центра
    double maxDist = sqrt(wGist * wGist + hGist * hGist);//радиус (окресность) от интересной точки

    for(int i = -maxDist - 1; i <= maxDist+1; i++){
        for(int j = -maxDist - 1; j <= maxDist+1; j++){

            //если точка попала в радиус, раскмдываем по корзинам
            if(pointDistance((double)i,0.0,(double)j,0.0) <= maxDist){

                double localPfi = Pfi.getPixel(point.x_ + i, point.y_ + j);
                double localBinCenter;

                //узнаём в какую часть дескриптора добавлять
                if(i > 0 && j > 0)   nGist = 0;     //верхний левый
                if(i <= 0 && j > 0)  nGist = 1;     //верхний правый
                if(i > 0 && j <= 0)  nGist = 2;     //нижний левый
                if(i <= 0 && j <= 0) nGist = 3;     //нижний правый
                nGist = 0;

                //nGist = ( abs(i + wGist + 1) / nGistBin) * (hGist / 2) + (abs(j + hGist + 1) / nGistBin);


                //узнаём номер текущей корзины
                bin = localPfi / binSize;

                //смежная корзина = следующая, для localPfi >= localBinCenter
                relatedBin = bin + 1;

                //узнаём центр текущей корзины
                localBinCenter = (double)bin * binSize + binSize / 2.0;

                //узнаём смежную корзину, для localPfi < localBinCenter (предидущая корзина)
                if(localPfi < localBinCenter) relatedBin = bin - 1;

                //узнаём расстояния
                disToCurCen = abs(localBinCenter - localPfi);
                disToRelCen = binSize - disToCurCen;

                //Обратнопропорционально распределяем Pfi между центрами
                //2-х смежных корзин, исходя из расстояния
                result.addGistValue(nGist, bin,        L.getPixel(point.x_ + i, point.y_ + j) * (1 - disToCurCen / binSize));
                result.addGistValue(nGist, relatedBin, L.getPixel(point.x_ + i, point.y_ + j) * (1 - disToRelCen / binSize));

            }
        }
    }

    //нормализуем
    result.biggestInTop();
    result.normalize();


    return result;
}
