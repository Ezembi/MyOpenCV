#include "DescriptorFactory.h"

DescriptorFactory::DescriptorFactory(const MyQImage &image)
{
    printf("Start: DescriptorFactory\n");
    L = image.sobel("All");
    Pfi.copy(image);

    octave_ = -1;
    level_ = -1;

    MyQImage dx = image.sobel("X");
    MyQImage dy = image.sobel("Y");

    for(int x = 0; x < image.getWidth(); x++){
        for(int y = 0; y < image.getHeight(); y++){
            Pfi.setPixel(x,y, atan2(dy.getPixel(x,y), dx.getPixel(x,y)) * 180.0 / M_PI + 180.0);
        }
    }
    printf("DescriptorFactory OK\n");

}

DescriptorFactory::DescriptorFactory(const MyQImage &image, int octave, int level)
{
    printf("Start: DescriptorFactory for octave #%d, level #%d (X = %d, Y = %d)\n", octave, level, image.getWidth(), image.getHeight());
    L = image.sobel("All");
    Pfi.copy(image);

    octave_ = octave;
    level_ = level;

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
    int koef = 2;
    double disToCurCen;                             //дистанция до текущего центра
    double disToRelCen;                             //дистанция до смежного центра
    int nPart = 4;                                  //кол-во частей гистограммы по 1-ой оси

    double side = 8.0;                                   //полусторона области для дескриптора

    //маштаб окрастности
    if(point.sigma_ != 0.0){
        side *= point.sigma_;
    }

    double maxDist = sqrt(side * side + side * side);//радиус (окресность) от интересной точки

    for(int i = -side * koef; i < side * koef; i++){
        for(int j = -side * koef; j < side * koef; j++){

            //поворачиваем точку окресности
            int newI = (i * cos((360.0 - point.alpha_) * M_PI / 180.0) - j * sin((360.0 - point.alpha_) * M_PI / 180.0) + 0.5);
            int newJ = (i * sin((360.0 - point.alpha_) * M_PI / 180.0) + j * cos((360.0 - point.alpha_) * M_PI / 180.0) + 0.5);

            if(pointDistance((double)newI,0.0,(double)newJ,0.0) < maxDist){

                double localPfi =  Pfi.getPixel(point.x_ + i, point.y_ + j) - point.alpha_;
                if(localPfi > 360) localPfi -= 360;
                if(localPfi < 0) localPfi += 360;

                double localBinCenter;

                int doubleMaxDist = side * 2;

                //узнаём номер гистограммы
                int Ix = newI + side;       //переводим х из -side <= x <= side в 0 <= x <= side * 2
                int Iy = newJ + side;       //переводим y из -side <= y <= side в 0 <= y <= side * 2

                if(newI < -side) Ix = 0;
                if(newJ < -side) Iy = 0;

                if(newI >= side) Ix = (side - 1) + side;
                if(newJ >= side) Iy = (side - 1) + side;

                int x = Ix / (doubleMaxDist / nPart);
                int y = Iy / (doubleMaxDist / nPart);

//                nGist = x * nPart + y;

                //узнаём номер текущей корзины
                bin = (localPfi / binSize + 0.5);

//                //смежная корзина = следующая, для localPfi >= localBinCenter
//                relatedBin = bin + 1;

//                //узнаём центр текущей корзины
//                localBinCenter = (double)bin * binSize + binSize / 2.0;

//                //узнаём смежную корзину, для localPfi < localBinCenter (предидущая корзина)
//                if(localPfi < localBinCenter) relatedBin = bin - 1;

//                //узнаём расстояния
//                disToCurCen = abs(localBinCenter - localPfi);
//                disToRelCen = binSize - disToCurCen;

//                //Обратнопропорционально распределяем L между центрами
//                //2-х смежных корзин, исходя из расстояния
//                result.addGistValue(nGist, bin,        L.getPixel(point.x_ + i, point.y_ + j) * (1 - disToCurCen / binSize));
//                result.addGistValue(nGist, relatedBin, L.getPixel(point.x_ + i, point.y_ + j) * (1 - disToRelCen / binSize));


                  /////////////////
                 //    Lab_7    //
                /////////////////
                double localL = L.getPixel(point.x_ + i, point.y_ + j);
                for(int xx = x - 1; xx <= x; xx++){

                    if(xx < 0 || xx >= nPart) continue;
                    int xCenter = gistSide * xx + gistSide / 2;
                    double Lx = abs(Ix - xCenter);
                    double Wx = 1 - Lx / gistSide;

                    for(int yy = y - 1; yy <= y; yy++){

                        if(yy < 0 || yy >= nPart) continue;
                        int yCenter = gistSide * yy + gistSide / 2;
                        double Ly = abs(Iy - yCenter);
                        double Wy = 1 - Ly / gistSide;

                        for(int bins = bin - 1; bins <= bin; bins ++){

                            localBinCenter = (double)bins * binSize + binSize / 2.0;
                            double La = abs(localBinCenter - localPfi);
                            double Wa = 1 - La / binSize;

                            nGist = xx * nPart + yy;
                            result.addGistValue(nGist, bins, localL * Wx * Wy * Wa);
                        }
                    }
                }
            }
        }
    }

    //нормализуем
    result.normalize();

    return result;
}

std::vector<Descriptor> DescriptorFactory::getDescriptors(const std::vector<InterestingPoint> &_points)
{
    std::vector<InterestingPoint> points = getOrientation(_points);
    std::vector<Descriptor> descriptors;
    for(int i = 0; i < points.size(); i++){
        if(octave_ == -1 && level_ == -1){
            descriptors.push_back(getDescrIntrPoint(points[i]));
        } else if(octave_ == points[i].octave_ && level_ == points[i].level_){
            descriptors.push_back(getDescrIntrPoint(points[i]));
        }
    }
    return descriptors;
}

std::vector<InterestingPoint> DescriptorFactory::getOrientation(const std::vector<InterestingPoint> &_points)
{
    std::vector<InterestingPoint> points;
    std::pair<InterestingPoint, InterestingPoint> result;

    for(int k = 0; k < _points.size(); k++){
        if(octave_ == -1 && level_ == -1){

            result = getOrientationIntrPoint(_points[k]);
            points.push_back(result.first);
            if(result.second.x_ != 0 && result.second.y_ != 0){
                points.push_back(result.second);
            }

        } else if(octave_ == _points[k].octave_ && level_ == _points[k].level_){

            result = getOrientationIntrPoint(_points[k]);
            points.push_back(result.first);
            if(result.second.x_ != 0 && result.second.y_ != 0){
                points.push_back(result.second);
            }

        }
    }

    return points;
}

std::pair<InterestingPoint, InterestingPoint> DescriptorFactory::getOrientationIntrPoint(const InterestingPoint &_points)
{
    std::pair<InterestingPoint, InterestingPoint> result;

    int localNBin = 36;
    double binSize = 360.0 / (double)localNBin;     //1/36 от 360 - размер в градусах одной корзины
    int bin;                                        //номер корзины
    int relatedBin;                                 //номер смежной корзины
    double disToCurCen;                             //дистанция до текущего центра
    double disToRelCen;                             //дистанция до смежного центра
    double localBin[localNBin];
    int side = 8;                                   //полусторона области для дескриптора
    double maxDist = sqrt(side * side + side * side);//радиус (окресность) от интересной точки
    int koef = 2;


    std::fill(localBin, localBin + localNBin, 0.0);

    for(int i = -side * koef; i < side * koef; i++){
        for(int j = -side * koef; j < side * koef; j++){

            //если точка попала в радиус, раскидываем по корзинам
            if(pointDistance((double)i,0.0,(double)j,0.0) < maxDist){

                double localPfi = Pfi.getPixel(_points.x_ + i, _points.y_ + j);
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
                localBin[bin       ] += L.getPixel(_points.x_ + i, _points.y_ + j) * (1 - disToCurCen / binSize);
                localBin[relatedBin] += L.getPixel(_points.x_ + i, _points.y_ + j) * (1 - disToRelCen / binSize);

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
    InterestingPoint point(
                _points.x_,
                _points.y_,
                _points.r_,
                _points.octave_,
                _points.level_,
                _points.value_,
                (double)maxI * binSize
                );
    result.first = point;

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
        InterestingPoint point(
                    _points.x_,
                    _points.y_,
                    _points.r_,
                    _points.octave_,
                    _points.level_,
                    _points.value_,
                    (double)maxI80 * binSize
                    );
        result.second = point;
    }


    return result;
}
