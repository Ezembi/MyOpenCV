#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include<vector>
#include<MyQImage.h>

class Descriptor
{
private:
    double gist_[nBin * nGistBin];       //гистограмма
    InterestingPoint point_;
public:
    Descriptor(InterestingPoint point);
    void setGistValue(int nGist, int bin, double value);
    void addGistValue(int nGist, int bin, double addValue);
    double getGistValue(int nGist, int bin) const;

    const InterestingPoint &getInterestingPoint() const;

    double getDistance(const Descriptor& descriptor) const;

    void normalize();
    double getVectorSize();

    void print();

};

//поиск "похожих" дескрипторов
std::vector<std::pair<InterestingPoint, InterestingPoint>> static getPairs(const std::vector<Descriptor> &descriptors1, const std::vector<Descriptor> &descriptors2){
    std::vector<std::pair<InterestingPoint, InterestingPoint>> pairs;

    for(int i = 0; i < descriptors1.size(); i++){

        double minDist1 = descriptors1[i].getDistance(descriptors2[0]);
        int minJ1 = 0;
        double minDist2;
        int minJ2;
        if(descriptors2.size() > 1){
            minDist2 = descriptors1[i].getDistance(descriptors2[1]);
            minJ2 = 1;
        } else {
            minDist2 = minDist1;
            minJ2 = 0;
        }

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
            std::pair<InterestingPoint, InterestingPoint> Pair;
            Pair.first = descriptors1[i].getInterestingPoint();
            Pair.second = descriptors2[minJ1].getInterestingPoint();

            pairs.push_back(Pair);
        }
    }
    return pairs;
}

#endif // DESCRIPTOR_H
