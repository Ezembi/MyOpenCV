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

MyQImage::MyQImage(MyQImage &&image)
{
    Image_ = std::move(image.Image_);

    height_ = image.height_;
    image.height_ = 0;

    width_ = image.width_;
    image.width_ = 0;

    format_ = image.format_;

    min_ = image.min_;
    image.min_ = 0;

    max_ = image.max_;
    image.max_ = 0;

    minOrigial_ = image.minOrigial_;
    image.minOrigial_ = 0;

    maxOrigial_ = image.maxOrigial_;
    image.maxOrigial_ = 0;

    printf("Move succesfully!\n");
}

MyQImage::~MyQImage()
{
    printf("Memory free\n");
}

const MyQImage &MyQImage::operator=(MyQImage &&image)
{
    Image_ = std::move(image.Image_);

    height_ = image.height_;
    width_ = image.width_;

    format_ = image.format_;

    min_ = image.min_;
    max_ = image.max_;

    minOrigial_ = image.minOrigial_;
    maxOrigial_ = image.maxOrigial_;

    printf("Operator = succesfully!\n");
    return *this;
}

const MyQImage &MyQImage::operator=(const MyQImage &image)
{
    //fixme реализовать
    /*Image_ = std::move(image.Image_);

    height_ = image.height_;
    width_ = image.width_;

    format_ = image.format_;

    min_ = image.min_;
    max_ = image.max_;

    minOrigial_ = image.minOrigial_;
    maxOrigial_ = image.maxOrigial_;*/
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
    Image_[getIndex(x , y, getWidth(), getHeight())] = color;
}

double MyQImage::getPixel(int x, int y) const
{
    return Image_[getIndex(x , y, getWidth(), getHeight())];
}

QRgb MyQImage::getColorPixel(int x, int y) const
{
    double color = getPixel(x,y);

    color = ((color - min_) / (max_ - min_)) * (maxOrigial_ - minOrigial_);
    return qRgb(color, color, color);

}

void MyQImage::copy(const MyQImage &imageForCopy)
{
    Image_ = std::make_unique<double[]>((imageForCopy.getWidth() * imageForCopy.getHeight()));
    setHeight(imageForCopy.getHeight());
    setWidth(imageForCopy.getWidth());

    format_ = imageForCopy.format_;
    minOrigial_ = imageForCopy.minOrigial_;
    maxOrigial_ = imageForCopy.maxOrigial_;

    for(int x = 0; x < getWidth(); x++){
        for(int y = 0; y < getHeight(); y++) {
            setPixel(x, y, imageForCopy.getPixel(x,y));
        }
    }

    printf("Copy succesfully!\n");
}

void MyQImage::resizeAndCopy(const MyQImage &imageForCopy, int newWidth, int newHeight)
{
    if(newWidth != 0 && newHeight != 0){
        Image_ = std::make_unique<double[]>((newWidth * newHeight));
        setHeight(newHeight);
        setWidth(newWidth);

        format_ = imageForCopy.format_;
        minOrigial_ = imageForCopy.minOrigial_;
        maxOrigial_ = imageForCopy.maxOrigial_;

        for(int x = 0; x < getWidth(); x++){
            for(int y = 0; y < getHeight(); y++) {
                setPixel(x, y, imageForCopy.getPixel(x * 2, y * 2));
            }
        }

        printf("Resize and copy succesfully!\n");
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

MyQImage MyQImage::blur() const
{
    printf("Blur\n");

    return convolution(&kernelBlur[0][0], 3, 3);
}

MyQImage MyQImage::sharpness() const
{
    printf("Sharpness\n");

    return convolution(&kernelSharpness[0][0], 3, 3);
}

MyQImage MyQImage::sobel(QString Param) const
{
    printf("Sobel\n");

    int u = 3,v = 3;

    if(Param == "X") {
        return convolution(&kernelSobelX[0][0],u,v);
    } else if(Param == "Y") {
        return convolution(&kernelSobelY[0][0],u,v);
    } else if(Param == "All"){
        return getSobelGradient(convolution(&kernelSobelX[0][0],u,v), convolution(&kernelSobelY[0][0],u,v));
    }
}

MyQImage MyQImage::getSobelGradient(const MyQImage &xImage, const MyQImage &yImage) const
{
    MyQImage resultImage(getWidth(), getHeight());

    for(int x = 0; x < getHeight(); x++){
        for(int y = 0; y < getWidth(); y++) {
            resultImage.setPixel(
                        x,
                        y,
                        sqrt(
                            (
                                xImage.getPixel(x,y) *
                                xImage.getPixel(x,y)
                                )
                            +
                            (
                                yImage.getPixel(x,y) *
                                yImage.getPixel(x,y)
                                )
                            )
                        );

        }
    }

    return resultImage;
}

MyQImage MyQImage::gaussianFilter(double sigma) const
{
    printf("GaussianFilter (sigma = %lf)\n", sigma);

    if(sigma != 0) {
        int widthHeight = 6 * qRound(sigma) + 1;
        double kernel[widthHeight][widthHeight];
        double kernelRow[widthHeight];
        double kernelColumn[widthHeight];

        getGaussianKernel(&kernel[0][0], widthHeight, widthHeight, sigma);

        for(int i = 0; i < widthHeight; i++){
            kernelRow[i] = kernel[0][i];
            kernelColumn[i] = kernel[i][0];
        }

        //return convolution(&kernel[0][0], widthHeight, widthHeight);
        return convolution(&kernelRow[0], &kernelColumn[0], widthHeight, widthHeight);
    }
    return *this;
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
            if(max < getPixel(x,y)) max = getPixel(x,y);
            if(min > getPixel(x,y)) min = getPixel(x,y);
        }
    }

    for(int i = 0; i < nPoint; i++){
        setPixel(rand()%getWidth(), rand()%getHeight(), max);
    }
}

std::vector<InterestingPoint> MyQImage::Moravec(int _u, int _v, int _dx, int _dy, double T) const
{
    printf("Start: Moravec\n");

    std::vector<InterestingPoint> points;

    int maxX = getWidth(), maxY = getHeight();

    std::unique_ptr<double[]> S = std::make_unique<double[]>(maxX * maxY);   //значение оператора

    const int u0 = - _u / 2;
    const int u1 = + _u / 2;
    const int v0 = - _v / 2;
    const int v1 = + _v / 2;

    double minC;
    double C;
    double color1, color2;

    for(int x = 0; x < maxX; x++){
        for(int y = 0; y < maxY; y++){

            for(int dx = -_dx; dx < _dx; dx++){
                for(int dy = -_dy; dy < _dy; dy++){

                    if(dx == 0 && dy == 0)
                        continue;
                    C = 0;
                    minC = 999999.0;

                    for(int u = u0; u < u1; u++){
                        for(int v = v0; v < v1; v++){

                            color1 = getPixel(x + u, y + v);
                            color2 = getPixel(x + u + dx, y + v + dy);
                            C += (color1 - color2) * (color1 - color2);
                        }
                    }

                    if(C < minC){
                        minC = C;
                    }
                }
            }

            S[getIndex(x , y, maxX, maxY)] = C;
        }
    }

    bool isMax;

    int _px = 1, _py = 1;

    for(int x = 0; x < maxX; x++){
        for(int y = 0; y < maxY; y++){

            if(S[getIndex(x , y, maxX, maxY)] > T){

                isMax = true;

                for(int px = -_px; px < _px && isMax; px++){
                    for(int py = -_py; py < _py && isMax; py++){

                        if(S[getIndex(x , y, maxX, maxY)] < S[getIndex(x + px, y + py, maxX, maxY)]){
                            isMax = false;
                        }
                    }
                }

                if(isMax){
                    InterestingPoint point(x, y, S[getIndex(x , y, maxX, maxY)]);
                    points.push_back(point);
                }
            }
        }
    }

    printf("Moravec OK\n");

    return points;
}

std::vector<InterestingPoint> MyQImage::Harris(int _dx, int _dy, double T, double k) const
{
    printf("Start: Harris\n");

    std::vector<InterestingPoint> points;

    MyQImage resultImage(getWidth(), getHeight());

    double maxX = getWidth();
    double maxY = getHeight();

    std::unique_ptr<double[]> S = std::make_unique<double[]>(maxX * maxY);   //значение оператора

    MyQImage SobelX = sobel("X");
    MyQImage SobelY = sobel("Y");

    double a, b, c, Ix, Iy;

    double det;
    double trace;
    double f;

    int _px = 1, _py = 1;
    bool isMax;

    MyQImage A(getWidth(), getHeight());
    MyQImage B(getWidth(), getHeight());
    MyQImage C(getWidth(), getHeight());

    for(int x = 0; x < maxX; x++){
        for(int y = 0; y < maxY; y++){

            a = 0;
            b = 0;
            c = 0;

            for(int dx = -_dx; dx < _dx; dx++){
                for(int dy = -_dy; dy < _dy; dy++){

                    Ix = SobelX.getPixel(x + dx, y + dy);
                    Iy = SobelY.getPixel(x + dx, y + dy);

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

            S[getIndex(x , y, maxX, maxY)] = f;
        }
    }

    for(int x = 0; x < maxX; x++){
        for(int y = 0; y < maxY; y++){

            if(S[getIndex(x , y, maxX, maxY)] > T){

                isMax = true;

                for(int px = -_px; px < _px && isMax; px++){
                    for(int py = -_py; py < _py && isMax; py++){
                        if(S[getIndex(x , y, maxX, maxY)] < S[getIndex(x + px, y + py, maxX, maxY)]){
                            isMax = false;
                        }
                    }
                }

                resultImage.setPixel(x, y, getPixel(x,y));

                if(isMax){
                    InterestingPoint point(x, y, S[getIndex(x , y, maxX, maxY)]);
                    points.push_back(point);
                }
            }
        }
    }

    printf("Harris OK\n");
    return points;

}

MyQImage MyQImage::convolution(const double *kernel, int u, int v) const
{
    MyQImage resultImage = *this;

    int x0, x1, y0, y1;

    int xMax = getWidth()-1, yMax = getHeight()-1;

    double resultPixel = 0.0;
    int n = u * v - 1;

    for(int i = 0; i < getWidth(); i++){
        for(int j = 0; j < getHeight(); j++) {
            x0 = i - (u / 2);
            x1 = i + (u / 2);
            y0 = j - (v / 2);
            y1 = j + (v / 2);
            resultPixel = 0.0;
            n = u * v - 1;

            for(int y = y0 ; y <= y1; y++) {
                for(int x = x0; x <= x1; x++, n--){
                    resultPixel += (kernel[n] * getPixel(reflect(x, xMax),reflect(y, yMax)));
                }
            }

            resultImage.setPixel(i,j,resultPixel);

        }
    }

    return resultImage;
}

MyQImage MyQImage::convolution(const double *row, const double *column, int u, int v) const
{
    MyQImage resultImage1(getWidth(), getHeight());
    MyQImage resultImage2(getWidth(), getHeight());
    resultImage1 = convolution(row, u, 1);
    resultImage2 = resultImage1.convolution(column, 1, v);
    return resultImage2;
}

QImage MyQImage::getQImage()
{
    QImage Picture(getWidth(),getHeight(),format_);
    for(int y = 0; y < getHeight(); y++){
        for(int x = 0; x < getWidth(); x++){
            Picture.setPixel(x, y, getColorPixel(x,y));
        }
    }

    return Picture;
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

            if(max_ < getPixel(x,y))
                max_ = getPixel(x,y);
            if(min_ > getPixel(x,y))
                min_ = getPixel(x,y);
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

void MyQImage::saveImage(QString file, std::vector<InterestingPoint> points)
{
    QImage Picture(getWidth(),getHeight(),format_);

    nInterestingPoint_ = points.size();

    max_ = getPixel(0,0);
    min_ = getPixel(0,0);

    for(int x = 0; x < getWidth()-1; x++){
        for(int y = 0; y < getHeight()-1; y++){

            if(max_ < getPixel(x,y))
                max_ = getPixel(x,y);
            if(min_ > getPixel(x,y))
                min_ = getPixel(x,y);
        }
    }

    for(int y = 0; y < getHeight(); y++){
        for(int x = 0; x < getWidth(); x++){
            Picture.setPixel(x, y, getColorPixel(x,y));
        }
    }

    QPainter painter;
    painter.begin(&Picture);
    painter.drawImage(QPoint(0,0), Picture);
    painter.setPen(Qt::red);

    for(int i = 0; i < points.size(); i++){
        painter.drawRect(points[i].x_ - 1, points[i].y_ - 1, 2, 2);
    }

    painter.end();

    Picture.save(file);

    printf("\nSave file OK! (%d x %d)\nCount interesting point = %d\nmin = %lf; max = %lf \nminOriginal = %lf; maxOriginal = %lf\n",getWidth(), getHeight(),nInterestingPoint_,min_, max_, minOrigial_, maxOrigial_);
}
