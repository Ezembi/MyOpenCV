#ifndef DEFINES
#define DEFINES

#define EXTRA_PIXEL -666.0        //255,0,0

int static reflect(int x, int MaxX)    //обработка краевых эффектов (отражение коэф зеркально от 0 и от ширины / высоты)
{
    if(x >= MaxX){
        return MaxX - (x - MaxX) - 1;
    }

    if(x < 0){
        return -1 * x;
    }

    return x;
}

#endif // DEFINES

