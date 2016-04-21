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
    int nPart = 2;                                  //кол-во частей гистограммы по 1-ой оси
    double disToCurCen;                             //дистанция до текущего центра
    double disToRelCen;                             //дистанция до смежного центра
    double maxDist = sqrt(wGist * wGist + hGist * hGist);//радиус (окресность) от интересной точки

    for(int i = -maxDist; i <= maxDist; i++){
        for(int j = -maxDist ; j <= maxDist; j++){

            if(pointDistance((double)i,0.0,(double)j,0.0) <= maxDist){

                //поворачиваем точку окресности
                int newI = i * cos(-point.alpha_ * M_PI / 180.0) - j * sin(-point.alpha_ * M_PI / 180.0);
                int newJ = i * sin(-point.alpha_ * M_PI / 180.0) + j * cos(-point.alpha_ * M_PI / 180.0);

                double localPfi =  Pfi.getPixel(point.x_ + newI, point.y_ + newJ) - point.alpha_;
                /*if(localPfi > 360) localPfi -= 360;*/
                if(localPfi < 0) localPfi += 360;

                double localBinCenter;

                int doubleMaxDist = (int)maxDist * 2;

                //сдвигаем -maxDist в 0,0;
                int Ix = i + maxDist;
                int Iy = j + maxDist;
                int x = Ix / (doubleMaxDist / nPart + 1);
                int y = Iy / (doubleMaxDist / nPart + 1);

                nGist = x * nPart + y;


                //----------------!!!----------------\\
                //вот тут как раз и ошибка с вычислением части дескриптора из за координаты 0
                //не понятно куда её пихать, если делать, что в дескрип. только одна часть,
                //но "длинная" из 36 корзин, то поворот считается норм
                nGist = 0;

                //узнаём номер текущей корзины
                bin = (localPfi / binSize + 0.5);

                //смежная корзина = следующая, для localPfi >= localBinCenter
                relatedBin = bin + 1;

                //узнаём центр текущей корзины
                localBinCenter = (double)bin * binSize + binSize / 2.0;

                //узнаём смежную корзину, для localPfi < localBinCenter (предидущая корзина)
                if(localPfi < localBinCenter) relatedBin = bin - 1;

                //узнаём расстояния
                disToCurCen = abs(localBinCenter - localPfi);
                disToRelCen = binSize - disToCurCen;

                //Обратнопропорционально распределяем L между центрами
                //2-х смежных корзин, исходя из расстояния
                result.addGistValue(nGist, bin,        L.getPixel(point.x_ + newI, point.y_ + newJ) * (1 - disToCurCen / binSize));
                result.addGistValue(nGist, relatedBin, L.getPixel(point.x_ + newI, point.y_ + newJ) * (1 - disToRelCen / binSize));
            }

        }
    }

    //нормализуем
    result.normalize();

    return result;
}

std::vector<InterestingPoint> DescriptorFactory::getOrientationIntrPoint(const std::vector<InterestingPoint> &_points)
{
    std::vector<InterestingPoint> points;

    int localNBin = 36;
    double binSize = 360.0 / (double)localNBin;     //1/36 от 360 - размер в градусах одной корзины
    int bin;                                        //номер корзины
    int relatedBin;                                 //номер смежной корзины
    double disToCurCen;                             //дистанция до текущего центра
    double disToRelCen;                             //дистанция до смежного центра
    double maxDist = sqrt(wGist * wGist + hGist * hGist);//радиус (окресность) от интересной точки
    double localBin[localNBin];

    for(int k = 0; k < _points.size(); k++){

        std::fill(localBin, localBin + localNBin, 0.0);

        for(int i = -maxDist - 1; i <= maxDist+1; i++){
            for(int j = -maxDist - 1; j <= maxDist+1; j++){

                //если точка попала в радиус, раскидываем по корзинам
                if(pointDistance((double)i,0.0,(double)j,0.0) <= maxDist){

                    double localPfi = Pfi.getPixel(_points[k].x_ + i, _points[k].y_ + j);
                    double localBinCenter;

                    //узнаём номер текущей корзины (+0.5 для более правильного округления)
                    bin = (localPfi / binSize + 0.5);

                    //смежная корзина = следующая, для localPfi >= localBinCenter
                    relatedBin = bin + 1;

                    //узнаём центр текущей корзины
                    localBinCenter = (double)bin * binSize + binSize / 2.0;

                    //узнаём смежную корзину, для localPfi < localBinCenter (предидущая корзина)
                    if(localPfi < localBinCenter) relatedBin = bin - 1;

                    //узнаём расстояния
                    disToCurCen = abs(localBinCenter - localPfi);
                    disToRelCen = binSize - disToCurCen;

                    //Обратнопропорционально распределяем L между центрами
                    //2-х смежных корзин, исходя из расстояния
                    localBin[getIndex(bin,        localNBin,0)] += L.getPixel(_points[k].x_ + i, _points[k].y_ + j) * (1 - disToCurCen / binSize);
                    localBin[getIndex(relatedBin, localNBin,0)] += L.getPixel(_points[k].x_ + i, _points[k].y_ + j) * (1 - disToRelCen / binSize);

                }
            }
        }

        //найдём максимальный
        double max = -1;
        int maxI = -1;
        for(int i = 0; i < localNBin; i++){
            if(localBin[i] > max){
                max = localBin[i];
                maxI = i;
            }
        }

        //добавляем
        InterestingPoint point(_points[k].x_, _points[k].y_, _points[k].value_, (double)maxI * binSize);
        points.push_back(point);

        //найдём 2-ой по максимуму
        double max80 = -1;
        int maxI80 = -1;
        for(int i = 0; i < localNBin; i++){
            if(i != maxI){
                if(localBin[i] > max80){
                    max80 = localBin[i];
                    maxI80 = i;
                }
            }
        }

        //если второй больше 80% от первого максимума, то добовляем вторую интересную точку
        if((max80 * 100.0) / max >= 80 && maxI80 != -1){
            InterestingPoint point(_points[k].x_, _points[k].y_, _points[k].value_, (double)maxI80 * binSize);
            points.push_back(point);
        }
    }

    return points;
}
