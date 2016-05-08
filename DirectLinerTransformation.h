#ifndef DIRECTLINERTRANSFORMATION_H
#define DIRECTLINERTRANSFORMATION_H
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <Assistant.h>

class DirectLinerTransformation
{
private:
    double h[9];
    void FillMatrix(gsl_matrix *A, const std::vector<std::pair<InterestingPoint, InterestingPoint>> pairs);

public:
    DirectLinerTransformation();
    DirectLinerTransformation(const std::vector<std::pair<InterestingPoint, InterestingPoint>> pairs);
};

#endif // DIRECTLINERTRANSFORMATION_H
