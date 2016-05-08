#include "DirectLinerTransformation.h"

DirectLinerTransformation::DirectLinerTransformation()
{
    for(int i = 0; i < 9; i++){
        h[i] = 0.0;
    }
}

DirectLinerTransformation::DirectLinerTransformation(const std::vector<std::pair<InterestingPoint, InterestingPoint> > pairs)
{
    const int j = 9;
    const int n = pairs.size();
    gsl_matrix *A = gsl_matrix_alloc(n * 2, j); //2*n x 9

    FillMatrix(A, pairs);                       //заполняем матрицу А

    gsl_matrix *ATA = gsl_matrix_alloc(j, j);   //А транспонированная на А (9х9)

    double alpha = 1.0;                         //х/з, во всех примерах = 1
    double betta = 0.0;                         //х/з, во всех примерах = 0

    gsl_blas_dgemm(
                CblasTrans,                     //"флаг" трансопнирования
                CblasNoTrans,                   //"флаг" без трансопнирования
                alpha,
                A,                              //первая матрица (для неё первый "флаг")
                A,                              //вторая матрица (для неё второй "флаг")
                betta,
                ATA                             //результирующая матрица
                );                              //перемножение матриц

    //http://www.gnu.org/software/gsl/manual/html_node/Singular-Value-Decomposition.html
    //SVD if ATA = (U S V^T) -> (U D U^T)
    //int gsl_linalg_SV_decomp (A, V, S, work)
    //On output the matrix A is replaced by U
    //The diagonal elements of the singular value matrix S are stored in the vector S
    //OMFG \o_O/
    gsl_matrix *U = gsl_matrix_alloc(j, j);
    gsl_vector *D = gsl_vector_alloc(j);
    gsl_vector *work = gsl_vector_alloc(j);
    gsl_linalg_SV_decomp(ATA, U, D, work);

    //h – столбец U соответствующий наименьшему сингулярному значению
    //D (обычно последнее значение, т.е. столбец)
    //h определен до масштаба, ограничение ||h|| = 1 или h22 = 1, т.е. последнее значение U
    //всё разделим на h22, чтобы h22 как раз и == 1
    double h22 = gsl_matrix_get(U, 8, 8);
    for(int i = 0; i < j; i++)
    {
        h[i] = gsl_matrix_get(U, i, 8) / h22;
    }

    gsl_matrix_free(A);
    gsl_matrix_free(ATA);
    gsl_matrix_free(U);
    gsl_vector_free(D);
    gsl_vector_free(work);
}

void DirectLinerTransformation::FillMatrix(gsl_matrix *A, const std::vector<std::pair<InterestingPoint, InterestingPoint> > pairs)
{
    //лекция 5, стр. 23, всё 1 в 1
    for(int i = 0; i < pairs.size(); i++){
        //x (чётные строки)
        gsl_matrix_set(A, i * 2, 0, pairs[i].first.x_);
        gsl_matrix_set(A, i * 2, 1, pairs[i].first.y_);
        gsl_matrix_set(A, i * 2, 2, 1);
        gsl_matrix_set(A, i * 2, 3, 0);
        gsl_matrix_set(A, i * 2, 4, 0);
        gsl_matrix_set(A, i * 2, 5, 0);
        gsl_matrix_set(A, i * 2, 6, -pairs[i].second.x_ * pairs[i].first.x_);
        gsl_matrix_set(A, i * 2, 7, -pairs[i].second.x_ * pairs[i].first.y_);
        gsl_matrix_set(A, i * 2, 8, -pairs[i].second.x_);

        //y (нечётные строки)
        gsl_matrix_set(A, i * 2 + 1, 0, 0);
        gsl_matrix_set(A, i * 2 + 1, 1, 0);
        gsl_matrix_set(A, i * 2 + 1, 2, 0);
        gsl_matrix_set(A, i * 2 + 1, 3, pairs[i].first.x_);
        gsl_matrix_set(A, i * 2 + 1, 4, pairs[i].first.y_);
        gsl_matrix_set(A, i * 2 + 1, 5, 1);
        gsl_matrix_set(A, i * 2 + 1, 6, -pairs[i].second.y_ * pairs[i].first.x_);
        gsl_matrix_set(A, i * 2 + 1, 7, -pairs[i].second.y_ * pairs[i].first.y_);
        gsl_matrix_set(A, i * 2 + 1, 8, -pairs[i].second.y_);
    }
}
