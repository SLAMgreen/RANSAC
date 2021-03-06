//
// Created by geoffrey on 31/12/15.
//

#include "linearmodel.h"


// Ordinary least squares regression estimate on a Point2f array of size nSample
LinearModel::LinearModel(std::vector<Point2f> dataSample) {
    pair<Point2f, Point2f> points;
    points.first = dataSample[0];
    points.second = dataSample[1];

    Point2f dir = points.second - points.first;
    normale = Point2f(dir.y/sqrt(dir.x*dir.x+dir.y*dir.y), -dir.x/sqrt(dir.x*dir.x+dir.y*dir.y));
    float c=points.first.y-(dir.y/dir.x)*points.first.x;
    Point2f oao(0,c);
    distanceToOrigin = oao.ddot(normale);
//    std::cout << distanceToOrigin;

}

LinearModel::LinearModel(Point2f normale, double distanceToOrigin) : normale(normale),
                                                                     distanceToOrigin(distanceToOrigin) { }

double LinearModel::distance(const Point2f &point) const {
    return abs(normale.ddot(point) - distanceToOrigin);
}

bool LinearModel::agree(const Point2f &point, double threshold) const {
    return threshold - distance(point) > 0;
}

Point2f LinearModel::getNormale() const {
    return normale;
}

double LinearModel::getDistanceToOrigin() const {
    return distanceToOrigin;
}

std::ostream &operator<<(std::ostream &ostream, const LinearModel &model) {
    ostream << "Normale: " << model.getNormale();
    ostream << ", Distance to origin: " << model.getDistanceToOrigin();
    return ostream;
}


LinearModel::LinearModel(std::vector<Point2f> dataSample, int size) {
    float nSamplef = (float) size;
    float var = 0.f;
    float sumX = 0.f;
    float sumY = 0.f;
    float meanX = 0.f;
    float meanY = 0.f;
    float covar = 0.f;
    float prodX = 0.f;
    float prodY = 0.f;


    for (int i = 0; i < nSamplef; i++) {
        var += pow((dataSample[i]).x, 2);
        sumX += (dataSample[i]).x;
        sumY += (dataSample[i]).y;
        prodX *= (dataSample[i]).x;
        prodY *= (dataSample[i]).y;
        covar += (dataSample[i]).x * (dataSample[i]).y;
    }
    //  std::cout << std::endl << "Var: " << var << std::endl;
    //  std::cout << "meanX: " << meanX << std::endl;
    //  std::cout << "meanY: " << meanY << std::endl;
    //  std::cout << "covar: " << covar << std::endl;

    var = var - pow(sumX, 2) / nSamplef;
    covar = covar - sumX * sumY / nSamplef;
    meanX = sumX / nSamplef;
    meanY = sumY / nSamplef;

    float b = 1.f;
    float a = covar / var;
    float c = -a * meanX + meanY;

    Point2f n(-b / sqrt(a * a + b * b), a / sqrt(a * a + b * b));
    normale = n;
    Point2f oao(0, c);
    distanceToOrigin = oao.ddot(normale);
}
