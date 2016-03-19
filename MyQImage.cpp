#include "MyQImage.h"

void MyQImage::setWidth(int width)
{
    width_ = width;
}

void MyQImage::setHeight(int height)
{
    height_ = height;
}

void MyQImage::loadImage(QString file)
{
    QImage Picture(file);
    format_ = Picture.format();
    minOrigial_ = 9999.0;
    maxOrigial_ = -9999.0;

    if(Picture.width() != 0 && Picture.height() != 0){

        setWidth(Picture.width());
        setHeight(Picture.height());

        Image_ = std::make_unique<double[]>(getWidth() * getHeight());


        for(int x = 0; x < getWidth(); x++){
            for(int y = 0; y < getHeight(); y++){
                setPixel(x,y,getMonoColor(Picture.pixel(x,y)));
            }
        }

        printf("Load succesfully! (%d x %d)\n",getWidth(), getHeight());
    } else {
        printf("Error: file not load!\n");
        exit(-1);
    }
}

MyQImage::MyQImage(QString file)
{
    loadImage(file);
}

MyQImage::MyQImage()
{
    Image_ = NULL;
}

MyQImage::MyQImage(int width, int height)
{
    if(width != 0 && height != 0){
        Image_ = std::make_unique<double[]>(width * height);
        setHeight(height);
        setWidth(width);
    }
}

MyQImage::MyQImage(const MyQImage &data, int newWidth, int newHeight)
{
    if(newWidth != 0 && newHeight != 0){
        Image_ = std::make_unique<double[]>((newWidth * newHeight));
        setHeight(newHeight);
        setWidth(newWidth);

        format_ = data.format_;
        minOrigial_ = data.minOrigial_;
        maxOrigial_ = data.maxOrigial_;

        int stepY = data.getHeight() / newHeight;
        int stepX = data.getWidth()  / newWidth;
        for(int newX = 0, x = 0; newX < newWidth ; newX++, x += stepX){
            for(int newY = 0, y = 0; newY < newHeight; newY++, y += stepY){
                setPixel(newX, newY, data.getPixel(x,y));
            }
        }
        printf("Resize succesfully!\n");
    }
}

MyQImage::MyQImage(const MyQImage& data)
{
    if(data.getWidth() != 0 && data.getHeight() != 0){
        Image_ = std::make_unique<double[]>(data.getWidth() * data.getHeight());
        setWidth(data.getWidth());
        setHeight(data.getHeight());

        format_ = data.format_;
        minOrigial_ = data.minOrigial_;
        maxOrigial_ = data.maxOrigial_;

        for(int x = 0; x < data.getWidth(); x++){
            for(int y = 0; y < data.getHeight(); y++){
                setPixel(x,y,data.getPixel(x,y));
            }
        }

        printf("Copy succesfully!\n");
    } else {
        printf("Error: Copy data is bad!\n");
        exit(-1);
    }
}

MyQImage::~MyQImage()
{
    printf("Memory free\n");
}

int MyQImage::getNormalNumber(double number) const
{
    return ((number - min_) / (max_ - min_)) * (255);

}

double MyQImage::getMonoColor(QRgb color)
{
    double colorOrigial = ((double)qRed(color) * 0.299 + (double)qGreen(color) * 0.587 + (double)qBlue(color) * 0.114);

    if(colorOrigial > maxOrigial_) maxOrigial_ = colorOrigial;
    if(colorOrigial < minOrigial_) minOrigial_ = colorOrigial;

    return colorOrigial / 255.0;
}

int MyQImage::getWidth() const
{
    return width_;
}

int MyQImage::getHeight() const
{
    return height_;
}

QImage::Format MyQImage::getFormat() const
{
    return format_;
}

void MyQImage::setPixel(int x, int y, double color)
{
    Image_[x * getHeight() + y] = color;
}

double MyQImage::getPixel(int x, int y) const
{
    return Image_[x * getHeight() + y];
}

QRgb MyQImage::getColorPixel(int x, int y)
{
    double color = getPixel(x,y);

    if(color != EXTRA_PIXEL){
        color = ((color - min_) / (max_ - min_)) * (maxOrigial_ - minOrigial_);
        return qRgb(color, color, color);
    } else {
        nInterestingPoint_++;
        printf("*");
        return qRgb(255, 0, 0);
    }

}

void MyQImage::swapPixel(int x1, int y1, int x2, int y2)
{
    double pixel = getPixel(x1,y1);
    setPixel(x1, y1, getPixel(x2,y2));
    setPixel(x2, y2, pixel);
}

void MyQImage::horizontalSwap()
{
    printf("Start: HorizontalSwap\n");
    for(int x = 0; x < getWidth(); x++){
        for(int y = 0; y < getHeight() / 2; y++) {
            swapPixel(x, y, x, (getHeight() - 1) - y);
        }
    }
    printf("HorizontalSwap OK!\n");
}

void MyQImage::verticalSwap()
{
    printf("Start: VerticalSwap\n");
    for(int x = 0; x < getWidth() / 2; x++){
        for(int y = 0; y < getHeight(); y++) {

            swapPixel(x, y,(getWidth() - 1) - x, y);
        }
    }
    printf("VerticalSwap OK!\n");
}

void MyQImage::blur(const MyQImage &originalImage)
{
    printf("Start: Blur\n");

    convolution(originalImage, &kernelBlur[0][0], 3, 3);

    printf("Blur OK!\n");
}

void MyQImage::sharpness(const MyQImage &originalImage)
{
    printf("Start: Sharpness\n");

    convolution(originalImage, &kernelSharpness[0][0], 3, 3);

    printf("Sharpness OK!\n");
}

void MyQImage::sobel(QString Param, const MyQImage &originalImage)
{
    printf("Start: Sobel\n");

    int u = 3,v = 3;

    MyQImage copyX = *this;
    MyQImage copyY = *this;


    if(Param == "X") {
        convolution(originalImage, &kernelSobelX[0][0],u,v);
    } else if(Param == "Y") {
        convolution(originalImage, &kernelSobelY[0][0],u,v);
    } else if(Param == "All"){
        copyX.convolution(originalImage, &kernelSobelX[0][0],u,v);
        copyY.convolution(originalImage, &kernelSobelY[0][0],u,v);

        for(int x = 0; x < getHeight(); x++){
            for(int y = 0; y < getWidth(); y++) {
                setPixel(x,y,getSobelGradient(copyX.getPixel(x,y),copyY.getPixel(x,y)));
            }
        }
    }
    printf("Sobel OK!\n");
}

void MyQImage::gaussianFilter(const MyQImage &originalImage, double sigma)
{
    printf("Start: GaussianFilter (sigma = %lf)\n", sigma);

    if(sigma != 0) {
        int widthHeight = 6 * qRound(sigma) + 1;
        double kernel[widthHeight][widthHeight];
        getGaussianKernel(&kernel[0][0], widthHeight, widthHeight, sigma);
        convolution(originalImage, &kernel[0][0], widthHeight, widthHeight);
    }

    printf("GaussianFilter OK!\n");
}

void MyQImage::addNoise(int nPoint)
{
    printf("Add noise %d pieces\n", nPoint);

    double max = getPixel(0,0);
    double min = getPixel(0,0);
    srand(time(NULL));

    for(int x = 0; x < getWidth()-1; x++){
        for(int y = 0; y < getHeight()-1; y++){
            if(max < getPixel(x,y) && getPixel(x,y) != EXTRA_PIXEL) max = getPixel(x,y);
            if(min > getPixel(x,y) && getPixel(x,y) != EXTRA_PIXEL) min = getPixel(x,y);
        }
    }

    for(int i = 0; i < nPoint; i++){
        setPixel(rand()%getWidth(), rand()%getHeight(), max);
    }
}

void MyQImage::resizeImage(int newHeight , int newWidth)
{
    int stepX, stepY;
    stepY = getHeight() / newWidth;
    stepX = getWidth() / newHeight;
    double color;
    for(int NewX = 0, x = 0; NewX < newHeight; NewX++, x += stepX){
        for(int NewY = 0, y = 0; NewY< newWidth; NewY++, y += stepY){
            color = getPixel(x,y);
            Image_[NewX * newHeight + NewY] = color;
        }
    }

    setHeight(newWidth);
    setWidth(newHeight);
}

void MyQImage::Moravec(int _u, int _v, int _dx, int _dy, int point_count, double T)
{
    printf("Start: Moravec\n");

    int maxX = getWidth(), maxY = getHeight();

    std::unique_ptr<double[]> S = std::make_unique<double[]>(maxX * maxY);   //значение оператора

    int u0, u1, v0, v1;
    double minC;
    double C;
    double color1, color2;

    for(int x = 0; x < maxX; x++){
        for(int y = 0; y < maxY; y++){
            u0 = x - _u / 2;
            u1 = x + _u / 2;
            v0 = y - _v / 2;
            v1 = y + _v / 2;

            for(int dx = -_dx; dx < _dx; dx++){
                for(int dy = -_dy; dy < _dy; dy++){
                    C = 0;
                    minC = -1;
                    for(int u = u0; u < u1; u++){
                        for(int v = v0; v < v1; v++){

                            color1 = getPixel(reflect(u, maxX), reflect(v, maxY));
                            color2 = getPixel(reflect(u + dx, maxX), reflect(v + dy, maxY));
                            C += (color1 - color2) * (color1 - color2);

                        }
                    }
                    if(C < minC) minC = C;

                }
            }
            S[x * maxX + y] = C;
        }
    }

    bool isMax;

    int _px, _py;

    _px = _py = ANMS(&S[0], maxX, maxY, point_count, T);

    for(int x = 0; x < maxX; x++){
        for(int y = 0; y < maxY; y++){
            isMax = true;
            for(int px = -_px; px < _px; px++){
                for(int py = -_py; py < _py; py++){
                    if(S[x * maxX + y] < S[reflect(x + px, maxX) * getWidth() + reflect(y + py, maxY)]){
                        isMax = false;
                    }
                }
            }

            if(isMax && S[x * maxX + y] > T){
                setPixel(x,y,EXTRA_PIXEL);
            }
        }
    }

    printf("Moravec OK!\n");
}

void MyQImage::Harris(int _dx, int _dy, int point_count, double T, double k)
{
    printf("Start: Harris\n");

    double maxX = getWidth();
    double maxY = getHeight();

    std::unique_ptr<double[]> S = std::make_unique<double[]>(maxX * maxY);   //значение оператора

    MyQImage SobelX = *this;
    MyQImage SobelY = *this;

    SobelX.sobel("X", *this);
    SobelY.sobel("Y", *this);

    double a, b, c, Ix, Iy;

    double det;
    double trace;
    double f;

    int _px, _py;
    bool isMax;

    MyQImage A = *this;
    MyQImage B = *this;
    MyQImage C = *this;

    for(int x = 0; x < maxX; x++){
        for(int y = 0; y < maxY; y++){

            a = 0;
            b = 0;
            c = 0;

            for(int dx = -_dx; dx < _dx; dx++){
                for(int dy = -_dy; dy < _dy; dy++){

                    Ix = SobelX.getPixel(reflect(x + dx, maxX), reflect(y + dy, maxY));
                    Iy = SobelY.getPixel(reflect(x + dx, maxX), reflect(y + dy, maxY));

                    a += Ix * Ix;
                    b += Ix * Iy;
                    c += Iy * Iy;
                }
            }

            A.setPixel(x, y, a);
            B.setPixel(x, y, b);
            C.setPixel(x, y, c);

        }
    }

    for(int x = 0; x < maxX; x++){
        for(int y = 0; y < maxY; y++){

            a = A.getPixel(x,y);
            b = B.getPixel(x,y);
            c = C.getPixel(x,y);

            det = a * c - b * b;
            trace = a + c;
            f = det - k * trace * trace;

            S[x * maxY + y] = f;
        }
    }

    _px = _py = ANMS(&S[0], maxX, maxY, point_count, T);

    for(int x = 0; x < maxX; x++){
        for(int y = 0; y < maxY; y++){

            isMax = true;

            for(int px = -_px; px < _px; px++){
                for(int py = -_py; py < _py; py++){

                    if(S[x * maxX + y] < S[reflect(x + px, maxX) * maxY + reflect(y + py, maxY)]){

                        isMax = false;

                    }
                }
            }

            if(isMax && S[x * maxY + y] > T){

                setPixel(x,y,EXTRA_PIXEL);

            }
        }
    }

    printf("Harris OK!\n");

}

void MyQImage::convolution(const MyQImage& originalImage, const double* kernel, int u, int v)
{
    int x0, x1, y0, y1;

    int xMax = originalImage.getWidth()-1, yMax = originalImage.getHeight()-1;

    double resultPixel = 0.0;
    int n = u * v - 1;

    for(int i = 0; i < originalImage.getWidth(); i++){
        for(int j = 0; j < originalImage.getHeight(); j++) {
            x0 = i - (u / 2);
            x1 = i + (u / 2);
            y0 = j - (v / 2);
            y1 = j + (v / 2);
            resultPixel = 0.0;
            n = u * v - 1;

            for(int y = y0 ; y <= y1; y++) {
                for(int x = x0; x <= x1; x++, n--){
                    resultPixel += (kernel[n] * originalImage.getPixel(reflect(x, xMax),reflect(y, yMax)));
                }
            }

            setPixel(i,j,resultPixel);

        }
    }
}

void MyQImage::convolution(const MyQImage &originalImage, const double *row, const double *column, int u, int v)
{

    convolution(originalImage, row, u, 1);
    convolution(*this, column, 1, v);

}

void MyQImage::saveImage(QString file)
{
    QImage Picture(getWidth(),getHeight(),format_);

    nInterestingPoint_ = 0;

    auto dminmax = std::minmax(&Image_[0], &Image_[0] + getHeight()* getWidth() );

    max_ = getPixel(0,0);
    min_ = getPixel(0,0);
    min_ = *dminmax.second;
    max_ = *dminmax.first;

    max_ = getPixel(0,0);
    min_ = getPixel(0,0);

    for(int x = 0; x < getWidth()-1; x++){
        for(int y = 0; y < getHeight()-1; y++){

            if(max_ < getPixel(x,y) && getPixel(x,y) != EXTRA_PIXEL) max_ = getPixel(x,y);
            if(min_ > getPixel(x,y) && getPixel(x,y) != EXTRA_PIXEL) min_ = getPixel(x,y);
        }
    }

    for(int y = 0; y < getHeight(); y++){
        for(int x = 0; x < getWidth(); x++){
            Picture.setPixel(x, y, getColorPixel(x,y));
        }
    }

    Picture.save(file);

    printf("\nSave file OK! (%d x %d)\nCount interesting point = %d\nmin = %lf; max = %lf \nminOriginal = %lf; maxOriginal = %lf\n",getWidth(), getHeight(),nInterestingPoint_,min_, max_, minOrigial_, maxOrigial_);
}

