#ifndef MYCOLOR_H
#define MYCOLOR_H

/**
 * @brief The MyColor class отвечает за цвет пикселя
 */
class MyColor{
private:
    int R;
    int G;
    int B;

public:
    MyColor();
    MyColor(int Red, int Green, int Blue);

    int getRed();
    int getGreen();
    int getBlue();

    void setColor(int Red, int Green, int Blue);

    void setRed(int Red);
    void setGreen(int Green);
    void setBlue(int Blue);

    int getMonoColor();
};

#endif // MYCOLOR_H
