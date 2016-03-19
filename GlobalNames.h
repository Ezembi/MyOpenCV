#ifndef DEFINES
#define DEFINES

#define EXTRA_PIXEL -666.0        //255,0,0

double const kernelBlur[3][3] = {
    {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
    {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
    {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
};

double const kernelSharpness[3][3] = {
    {-1.0 / 9.0, -1.0 / 9.0, -1.0 / 9.0},
    {-1.0 / 9.0, 17.0 / 9.0, -1.0 / 9.0},
    {-1.0 / 9.0, -1.0 / 9.0, -1.0 / 9.0}
};

double const kernelSobelX[3][3] = {
    {-1.0, 0.0, 1.0},
    {-2.0, 0.0, 2.0},
    {-1.0, 0.0, 1.0}
};

double const kernelSobelY[3][3] = {
    {-1.0,-2.0,-1.0},
    { 0.0, 0.0, 0.0},
    { 1.0, 2.0, 1.0}
};

#endif // DEFINES

