#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <climits>

// Colour palette for terminal output
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define ERROR_COLOR "\033[38;5;124m"  // Crimson red for errors
#define ARRAY_BLUE  "\033[34m"        // Blue for array elements


class NumpyArray {
private:
    double* data;
    int rows;
    int cols;
    int totalSize;
    int dimensions;
public:

    // Constructor for 1D array
    NumpyArray(int);

    // Constructor for 2D array
    NumpyArray(int, int);

    // Copy constructor
    NumpyArray(const NumpyArray&);

    // Destructor
    ~NumpyArray() {
        delete[] data;
    }

    // Basic array operations
    void printArray() const;
    NumpyArray& operator=(const NumpyArray&);

    double& operator()(int);
    const double& operator()(int) const;

    double& operator()(int, int);
    const double& operator()(int, int) const;

    // Array information
    std::string shape() const;
    int size() const;
    int ndim() const;

    // Numerical operations
    double sum() const;
    double mean() const;
    double min() const;
    double max() const;

    // Array creation
    static NumpyArray zeros(int, int);
    static NumpyArray ones(int, int);
    static NumpyArray eye(int);
    static NumpyArray arange(int, int, int = 1);

    // 2D slicing
    NumpyArray slice(int, int, int, int) const;

    // Stream output
    friend std::ostream& operator<<(std::ostream&, const NumpyArray&);

    int getRows() const;
    int getCols() const;
};


// Class NumpyArray methods:


// Constructor for 1D array
NumpyArray::NumpyArray(int size) {

    if (size < 0) {
        throw std::invalid_argument("Array size cannot be negative");
    }
    rows = 1;
    cols = size;
    totalSize = size;
    dimensions = 1;

    data = new double[totalSize];

    // Initialise the array with zeros
    for (int i = 0; i < totalSize; i++) {
        data[i] = 0.0;
    }
}


// Constructor for 2D array
NumpyArray::NumpyArray(int r, int c) {

    if (r < 0 || c < 0) {
        throw std::invalid_argument("Array dimensions cannot be negative");
    }

    // Prevent overflow while calculating total size
    if (r != 0 && c > INT_MAX / r) {
        throw std::overflow_error("Array is too large");
    }

    rows = r;
    cols = c;
    totalSize = r * c;
    dimensions = 2;

    data = new double[totalSize];

    // Initialise the array with zeros
    for (int i = 0; i < totalSize; i++) {
        data[i] = 0.0;
    }
}


// Copy constructor
NumpyArray::NumpyArray(const NumpyArray& obj) {

    // Copy the object's dimensions
    rows = obj.rows;
    cols = obj.cols;
    totalSize = obj.totalSize;
    dimensions = obj.dimensions;

    // Allocate NEW independent memory on the heap
    data = new double[totalSize];

    for (int i = 0; i < totalSize; i++) {
        data[i] = obj.data[i];
    }
}


// Print the array
void NumpyArray::printArray() const {
    std::cout << *this;
}


// Assignment operator
NumpyArray& NumpyArray::operator=(const NumpyArray& obj) {

    // Prevent self-assignment (eg: a = a)
    if (this == &obj) {
        return *this;
    }

    // Allocate new memory and copy data inside
    double* newData = new double[obj.totalSize];

    for (int i = 0; i < obj.totalSize; i++) {
        newData[i] = obj.data[i];
    }

    // Clean old memory
    delete[] data;

    // Update dimensions and data
    rows = obj.rows;
    cols = obj.cols;
    totalSize = obj.totalSize;
    dimensions = obj.dimensions;
    data = newData;

    return *this;
}


// 1D indexing operator
double& NumpyArray::operator()(int i) {

    if (i < 0 || i >= totalSize) {
        throw std::out_of_range("Index out of bounds");
    }

    return data[i];    // Return the element at index i
}


// 1D indexing operator for constant arrays
const double& NumpyArray::operator()(int i) const {

    if (i < 0 || i >= totalSize) {
        throw std::out_of_range("Index out of bounds");
    }

    return data[i];
}


// 2D indexing operator
double& NumpyArray::operator()(int i, int j) {

    if (i < 0 || i >= rows || j < 0 || j >= cols) {
        throw std::out_of_range("Index out of bounds");
    }

    return data[i * cols + j];    // Return the element at (i,j)
}


// 2D indexing operator for constant arrays
const double& NumpyArray::operator()(int i, int j) const {

    if (i < 0 || i >= rows || j < 0 || j >= cols) {
        throw std::out_of_range("Index out of bounds");
    }

    return data[i * cols + j];
}

// Return the shape of the array
std::string NumpyArray::shape() const {

    // Printing the result in a tuple for a more intuitive and pythonic way
    if (dimensions == 1) {
        return "(" + std::to_string(cols) + ",)";
    }
    else {
        return "(" + std::to_string(rows) + ", " + std::to_string(cols) + ")";
    }
}

// Return the number of dimensions
int NumpyArray::ndim() const {
    return dimensions;
}

// Return the total number of elements
int NumpyArray::size() const {
    return totalSize;
}



// Calculate the sum of all elements
double NumpyArray::sum() const {

    double total = 0;

    // Works for both 1D and 2D arrays
    for (int i = 0; i < totalSize; i++) {
        total += data[i];
    }

    return total;
}


// Calculate the mean of all elements
double NumpyArray::mean() const {

    if (totalSize == 0) {
        throw std::runtime_error("Cannot calculate mean of an empty array");
    }

    return sum() / totalSize;
}


// Find the minimum element
double NumpyArray::min() const {

    if (totalSize == 0) {
        throw std::runtime_error("Cannot find minimum of an empty array");
    }

    double minimum = data[0];

    for (int i = 1; i < totalSize; i++) {
        if (data[i] < minimum) {
            minimum = data[i];
        }
    }

    return minimum;
}


// Find the maximum element
double NumpyArray::max() const {

    if (totalSize == 0) {
        throw std::runtime_error("Cannot find maximum of an empty array");
    }

    double maximum = data[0];

    for (int i = 1; i < totalSize; i++) {
        if (data[i] > maximum) {
            maximum = data[i];
        }
    }

    return maximum;
}


// Create an array filled with zeros
NumpyArray NumpyArray::zeros(int rows, int cols) {

    return NumpyArray(rows, cols);
}


// Create an array filled with ones
NumpyArray NumpyArray::ones(int rows, int cols) {

    NumpyArray result(rows, cols);

    for (int i = 0; i < result.totalSize; i++) {
        result.data[i] = 1;
    }

    return result;
}


// Create an identity matrix
NumpyArray NumpyArray::eye(int size) {

    NumpyArray result(size, size);

    for (int i = 0; i < size; i++) {
        result.data[i * size + i] = 1;
    }

    return result;
}


// Create an array containing a sequence of numbers
NumpyArray NumpyArray::arange(int start, int stop, int step) {

    if (step <= 0) {
        throw std::invalid_argument("Step must be positive");
    }

    long long count = 0;

    if (start < stop) {
        count = ((long long)stop - start + step - 1) / step;
    }
    if (count > INT_MAX) {
        throw std::overflow_error("Array is too large");
    }
    NumpyArray result((int)count);
    long long value = start;
    for (int i = 0; i < result.size(); i++) {
        result(i) = value;
        value += step;
    }
    return result;
}


// Extract a portion of a 2D array
NumpyArray NumpyArray::slice(int rowStart, int rowStop, int colStart, int colStop) const {

    // Check for invalid slice indices
    if (rowStart < 0 || rowStop > rows || rowStart >= rowStop ||
        colStart < 0 || colStop > cols || colStart >= colStop) {

        throw std::invalid_argument("Invalid slice indices");
    }

    int newRows = rowStop - rowStart;
    int newCols = colStop - colStart;

    NumpyArray result(newRows, newCols);

    for (int i = rowStart; i < rowStop; i++) {
        for (int j = colStart; j < colStop; j++) {

            result(i - rowStart, j - colStart) = data[i * cols + j];
        }
    }

    return result;
}


// Stream insertion operator
std::ostream& operator<<(std::ostream& dout, const NumpyArray& obj) {

    dout << BOLD << "[\n" << RESET;

    for (int i = 0; i < obj.rows; i++) {

        dout << "  [";

        for (int j = 0; j < obj.cols; j++) {

            int index = (i * obj.cols) + j;

            dout << ARRAY_BLUE << obj.data[index] << RESET;

            if (j < obj.cols - 1) {
                dout << ", ";
            }
        }

        dout << "]\n";
    }

    dout << BOLD << "]\n" << RESET;

    return dout;
}
int NumpyArray::getRows() const {
    return rows;
}

int NumpyArray::getCols() const {
    return cols;
}

// class to store the image
// every images's pixel is a combination of RGB
// we can use NumpyArrays to store each of an images's channel info

class Image {
private:
    int width;
    int height;

    NumpyArray red;
    NumpyArray green;
    NumpyArray blue;

public:
    // Constructor
    Image(int w, int h);
    Image(int w, int h,const NumpyArray&,const NumpyArray&,const NumpyArray&);

    // Pixel accessing
    std::vector<double> getPixel(int row, int col)const;
    void setPixel(int row, int col, double r, double g, double b);

    // Get image dimensions
    int getWidth()const;
    int getHeight()const;

    // Get RGB channels
    const NumpyArray& getRedChannel()const;
    const NumpyArray& getGreenChannel()const;
    const NumpyArray& getBlueChannel()const;
};

// Constructor
Image::Image(int w, int h) : width(w), height(h), red(h, w), green(h, w), blue(h, w) {

    if (w <= 0 || h <= 0) {
        throw std::invalid_argument("Image dimensions must be positive");
    }
}
// Constructor using existing RGB channels
Image::Image(int w, int h, const NumpyArray& r, const NumpyArray& g, const NumpyArray& b) : width(w), height(h), red(r), green(g), blue(b) {

    if (w <= 0 || h <= 0) {
        throw std::invalid_argument("Image dimensions must be positive");
    }

    if (r.getRows() != h || r.getCols() != w ||
        g.getRows() != h || g.getCols() != w ||
        b.getRows() != h || b.getCols() != w) {

        throw std::invalid_argument("RGB channel dimensions do not match image dimensions");
    }
}

// Get red channel
const NumpyArray& Image::getRedChannel() const {
    return red;
}

// Get green channel
const NumpyArray& Image::getGreenChannel() const {
    return green;
}

// Get blue channel
const NumpyArray& Image::getBlueChannel() const{
    return blue;
}

// Set RGB values of a pixel
void Image::setPixel(int row, int col, double r, double g, double b) {

    if (row < 0 || row >= height || col < 0 || col >= width) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }

    // Keep colour values within the range 0-255
    if (r < 0) r = 0;
    if (r > 255) r = 255;

    if (g < 0) g = 0;
    if (g > 255) g = 255;

    if (b < 0) b = 0;
    if (b > 255) b = 255;

    red(row, col) = r;
    green(row, col) = g;
    blue(row, col) = b;
}

// Get RGB values of a pixel
std::vector<double> Image::getPixel(int row, int col) const {

    if (row < 0 || row >= height || col < 0 || col >= width) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }

    return {
        red(row, col),
        green(row, col),
        blue(row, col)
    };
}

// Get width
int Image::getWidth() const {
    return width;
}

// Get height
int Image::getHeight() const {
    return height;
}


class ImageIO{
public:
    //We don't need to create an ImageIO object just to load an image. Thus we make it static.
    // it returns an image.
    static Image loadPPM(std::string);
    static void display(const Image&);
};
Image ImageIO::loadPPM(std::string filename) {

    std::ifstream file(filename);

    // Check whether file opened successfully
    if (!file) {
        throw std::runtime_error("Could not open image file");
    }

    std::string format;
    int width, height, maxValue;

    // Read PPM header
    file >> format;

    if (format != "P3") {
        throw std::runtime_error("Only P3 PPM format is supported");
    }

    file >> width >> height;
    file >> maxValue;

    if (!file || width <= 0 || height <= 0 ||
        maxValue <= 0 || maxValue > 65535) {
        throw std::runtime_error("Invalid PPM header");
    }

    // Create image object
    Image img(width, height);

    // Read pixel data
    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {

            int r, g, b;

            file >> r >> g >> b;

            if (!file || r < 0 || r > maxValue ||
                g < 0 || g > maxValue ||
                b < 0 || b > maxValue) {

                throw std::runtime_error("Invalid pixel data");
            }

            // Convert values to the 0-255 range
            r = (r * 255 + maxValue / 2) / maxValue;
            g = (g * 255 + maxValue / 2) / maxValue;
            b = (b * 255 + maxValue / 2) / maxValue;

            img.setPixel(i, j, r, g, b);
        }
    }

    return img;
}
void ImageIO::display(const Image& img) {

    int width = img.getWidth();
    int height = img.getHeight();

    // Maximum terminal display dimensions
    int maxWidth = 100;
    int maxHeight = 35;

    // Calculate scaling factor
    double scale = 1.0;

    if (width > maxWidth) {
        scale = (double)maxWidth / width;
    }

    if (height * scale > maxHeight) {
        scale = (double)maxHeight / height;
    }

    // Calculate new dimensions
    int newWidth = (int)(width * scale);
    int newHeight = (int)(height * scale);

    if (newWidth < 1) newWidth = 1;
    if (newHeight < 1) newHeight = 1;

    // Display the scaled image
    for (int i = 0; i < newHeight; i++) {

        for (int j = 0; j < newWidth; j++) {

            // Find corresponding pixel in original image
            int originalRow = i * height / newHeight;
            int originalCol = j * width / newWidth;

            std::vector<double> pixel =
                img.getPixel(originalRow, originalCol);

            int r = pixel[0];
            int g = pixel[1];
            int b = pixel[2];

            std::cout << "\033[48;2;"
                      << r << ";"
                      << g << ";"
                      << b << "m  \033[0m";
        }

        std::cout << std::endl;
    }
}

// filters:
// greyscale,brightness,invert. these perform different opeartions
// but share a common idea ie:
// they take an image, modify it and produce a processed image
// thus instead of implementing every filter, we make a common class 
// named ImgeFilter and add a pure virtual function to make it abstract!
// can also show polymorphism!
class ImageFilter{
public:
    virtual void apply(Image&) = 0; // pure virtual function
    virtual ~ImageFilter() {} // virtual destructor
};

/* to convert into grayscale, the intensity value ultimately 
   depends on the three channels as:

        Gray = 0.299*Red + 0.587*Green + 0.114*Blue

   this is mainly because naturally the eyes are more sensitive to green
   as compared to red and more sensitive to red as compared to blue.
   so the green contributes more to the perceived brightness!*/

class GrayScaleFilter : public ImageFilter{
public:
    void apply(Image&)override;
};
void GrayScaleFilter::apply(Image& img) {

    int width = img.getWidth();
    int height = img.getHeight();

    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {

            std::vector<double> pixel = img.getPixel(i, j);

            double r = pixel[0];
            double g = pixel[1];
            double b = pixel[2];

            double gray = 0.299 * r + 0.587 * g + 0.114 * b;

            img.setPixel(i, j, gray, gray, gray);
        }
    }
}


/* implementing Brightness Filter:
   to implement brightness filter, we just have to add a constant
   to our existing channels as: let 'k' be the constant then:

   c'(new channel) = c(original channel) + k  for all channels

*/

class BrightnessFilter : public ImageFilter {
private:
    double brightness;
public:
    BrightnessFilter(double b) : brightness(b) {}
    void apply(Image&) override;
};
void BrightnessFilter::apply(Image& img) {

    int width = img.getWidth();
    int height = img.getHeight();

    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {

            std::vector<double> pixel = img.getPixel(i, j);

            double r = pixel[0] + brightness;
            double g = pixel[1] + brightness;
            double b = pixel[2] + brightness;

            // Clamp RGB values between 0 and 255
            if (r > 255) r = 255;
            if (r < 0) r = 0;

            if (g > 255) g = 255;
            if (g < 0) g = 0;

            if (b > 255) b = 255;
            if (b < 0) b = 0;

            img.setPixel(i, j, r, g, b);
        }
    }
}
// invert filter:
// intuitively its just the complement of channels
// mathematically, c'(new channel) = 255 - c(old channel) 
class InvertFilter : public ImageFilter {
public:
    void apply(Image&) override;
};
void InvertFilter::apply(Image& img) {

    int width = img.getWidth();
    int height = img.getHeight();

    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {

            std::vector<double> pixel = img.getPixel(i, j);

            double r = 255 - pixel[0];
            double g = 255 - pixel[1];
            double b = 255 - pixel[2];

            img.setPixel(i, j, r, g, b);
        }
    }
}

// abstract base class for image transformation operations. 
// like cropping, rotationf, flipping
// this doesnt change the colors/pixel values, instead it chnages the structure/dimensions
class ImageTransform{
public:
    virtual void apply(Image&) = 0; // pure virtual function
    virtual ~ImageTransform() {} // virtual destructor    
};

class CropTransform : public ImageTransform {
private:
    int rowStart;
    int rowStop;
    int colStart;
    int colStop;
public:
    CropTransform(int,int,int,int);
    void apply(Image&) override;
};

CropTransform::CropTransform(int rs,int re,int cs,int ce) : rowStart(rs),rowStop(re),colStart(cs),colStop(ce) {}

void CropTransform::apply(Image& img) {
    // validating coordinates
    if (rowStart < 0 || rowStop > img.getHeight() ||
        colStart < 0 || colStop > img.getWidth() ||
        rowStart >= rowStop || colStart >= colStop) {

        throw std::out_of_range("Invalid cropping dimensions");
    }

    // cropping individual RGB channels
    NumpyArray croppedRed = img.getRedChannel().slice(rowStart,rowStop,colStart,colStop);
    NumpyArray croppedGreen = img.getGreenChannel().slice(rowStart,rowStop,colStart,colStop);
    NumpyArray croppedBlue = img.getBlueChannel().slice(rowStart,rowStop,colStart,colStop);

    // creating cropped image
    Image cropped(colStop-colStart,rowStop-rowStart,croppedRed,croppedGreen,croppedBlue);

    // replace the origiinal image
    img = cropped;
}

class FlipTransform : public ImageTransform {
private:
    int axis;
public:
    FlipTransform(int);
    void apply(Image&) override;
};
FlipTransform::FlipTransform(int a) : axis(a) {
    if(axis != 0 && axis != 1)throw std::invalid_argument("Flip axis must be 0 or 1!");
}
void FlipTransform::apply(Image& img) {
    int width = img.getWidth();
    int height = img.getHeight();

    Image flipped(width,height);

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            std::vector<double> pixel = img.getPixel(i,j);
            if(axis==0) flipped.setPixel(height-i-1,j,pixel[0],pixel[1],pixel[2]);
            else flipped.setPixel(i,width-j-1,pixel[0],pixel[1],pixel[2]);
        }
    }
    img = flipped;
}
// rotate an image by angles 90,180,270 only!!
class RotateTransform : public ImageTransform {
private:
    int angle; // must be 90,180,270 strictly!
public:
    RotateTransform(int);
    void apply(Image&) override;
}; 
RotateTransform::RotateTransform(int a) : angle(a) {
    if(angle != 90 && angle != 180 && angle != 270) throw std::invalid_argument("Rotation angle must be 90/180/270!");
}
void RotateTransform::apply(Image& img) {

    int width = img.getWidth();
    int height = img.getHeight();

    int newWidth = width;
    int newHeight = height;

    if (angle == 90 || angle == 270) {
        newWidth = height;
        newHeight = width;
    }

    Image rotated(newWidth, newHeight);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            std::vector<double> pixel = img.getPixel(i, j);

            int newRow, newCol;

            if (angle == 90) {
                newRow = j;
                newCol = height - 1 - i;
            }
            else if (angle == 180) {
                newRow = height - 1 - i;
                newCol = width - 1 - j;
            }
            else {
                newRow = width - 1 - j;
                newCol = i;
            }

            rotated.setPixel(newRow, newCol,
                             pixel[0], pixel[1], pixel[2]);
        }
    }

    img = rotated;
}

class ImageAnalyzer {
public:
    static double getAverageRed(const Image& img);
    static double getAverageGreen(const Image& img);
    static double getAverageBlue(const Image& img);

    static double getAverageBrightness(const Image& img);

    static double getMinBrightness(const Image& img);
    static double getMaxBrightness(const Image& img);

    static void showImageInfo(const Image& img);
};
double ImageAnalyzer::getAverageRed(const Image& img) {
    return img.getRedChannel().mean();
}

double ImageAnalyzer::getAverageGreen(const Image& img) {
    return img.getGreenChannel().mean();
}

double ImageAnalyzer::getAverageBlue(const Image& img) {
    return img.getBlueChannel().mean();
}

double ImageAnalyzer::getAverageBrightness(const Image& img) {

    double total = 0;

    for (int i = 0; i < img.getHeight(); i++) {
        for (int j = 0; j < img.getWidth(); j++) {

            std::vector<double> pixel = img.getPixel(i, j);

            double brightness =
                0.299 * pixel[0] +
                0.587 * pixel[1] +
                0.114 * pixel[2];

            total += brightness;
        }
    }

    int totalPixels = img.getWidth() * img.getHeight();

    return total / totalPixels;
}

double ImageAnalyzer::getMinBrightness(const Image& img) {

    double minimum = 255;

    for (int i = 0; i < img.getHeight(); i++) {
        for (int j = 0; j < img.getWidth(); j++) {

            std::vector<double> pixel = img.getPixel(i, j);

            double brightness =
                0.299 * pixel[0] +
                0.587 * pixel[1] +
                0.114 * pixel[2];

            if (brightness < minimum) {
                minimum = brightness;
            }
        }
    }

    return minimum;
}


double ImageAnalyzer::getMaxBrightness(const Image& img) {

    double maximum = 0;

    for (int i = 0; i < img.getHeight(); i++) {
        for (int j = 0; j < img.getWidth(); j++) {

            std::vector<double> pixel = img.getPixel(i, j);

            double brightness =
                0.299 * pixel[0] +
                0.587 * pixel[1] +
                0.114 * pixel[2];

            if (brightness > maximum) {
                maximum = brightness;
            }
        }
    }

    return maximum;
}

void ImageAnalyzer::showImageInfo(const Image& img) {

    std::cout << "\nIMAGE\n";

    std::cout << "  Format        : PPM (P3)\n";

    std::cout << "  Dimensions    : "
              << img.getWidth() << " x "
              << img.getHeight() << "\n";

    std::cout << "  Channels      : RGB\n";

    std::cout << "  Total pixels  : "
              << img.getWidth() * img.getHeight() << "\n";


    std::cout << "\nANALYSIS\n";

    std::cout << "  Avg. red      : "
              << getAverageRed(img) << "\n";

    std::cout << "  Avg. green    : "
              << getAverageGreen(img) << "\n";

    std::cout << "  Avg. blue     : "
              << getAverageBlue(img) << "\n";

    std::cout << "  Brightness    : "
              << getAverageBrightness(img) << "\n";
}

int main() {

    try {

        Image img = ImageIO::loadPPM("output.ppm");

        std::cout << BOLD << ARRAY_BLUE
                  << "\nWelcome to sudo Picasso!\n"
                  << RESET;

        std::cout << "Image loaded: output.ppm\n";
        std::cout << "Resolution: " << img.getWidth()
                  << " x " << img.getHeight() << "\n";

        ImageIO::display(img);


        // Available filters
        GrayScaleFilter grayScale;
        BrightnessFilter brightness(50);
        InvertFilter invert;

        std::vector<ImageFilter*> filterList = {
            &grayScale, &brightness, &invert
        };


        // Available transformations
        FlipTransform verticalFlip(0);
        FlipTransform horizontalFlip(1);

        RotateTransform rotate90(90);
        RotateTransform rotate180(180);
        RotateTransform rotate270(270);

        std::vector<ImageTransform*> transformList = {
            &verticalFlip, &horizontalFlip,
            &rotate90, &rotate180, &rotate270
        };


        int choice;

        while (true) {

            std::cout << "\n\nImage editing options:\n\n";

            std::cout << "  Filters\n";
            std::cout << "  1. Grayscale\n";
            std::cout << "  2. Increase brightness\n";
            std::cout << "  3. Invert colors\n\n";

            std::cout << "  Transformations\n";
            std::cout << "  4. Flip vertically\n";
            std::cout << "  5. Flip horizontally\n";
            std::cout << "  6. Rotate 90 degrees\n";
            std::cout << "  7. Rotate 180 degrees\n";
            std::cout << "  8. Rotate 270 degrees\n";
            std::cout << "  9. Crop image\n\n";

            std::cout << "  Other\n";
            std::cout << "  10. Image information\n";
            std::cout << "  11. Display image\n";
            std::cout << "  0. Exit\n";

            std::cout << "\nChoice: ";
            std::cin >> choice;


            if (choice == 0) {
                std::cout << "\nClosing sudo Picasso. Goodbye!\n";
                break;
            }


            if (choice >= 1 && choice <= 3) {

                filterList[choice - 1]->apply(img);

                std::cout << "\nFilter applied.\n";
                ImageIO::display(img);
            }

            else if (choice >= 4 && choice <= 8) {

                transformList[choice - 4]->apply(img);

                std::cout << "\nTransformation applied.\n";
                ImageIO::display(img);
            }

            else if (choice == 9) {

                int rowStart, rowEnd, colStart, colEnd;

                std::cout << "\nStarting row: ";
                std::cin >> rowStart;

                std::cout << "Ending row (exclusive): ";
                std::cin >> rowEnd;

                std::cout << "Starting column: ";
                std::cin >> colStart;

                std::cout << "Ending column (exclusive): ";
                std::cin >> colEnd;

                CropTransform crop(rowStart, rowEnd, colStart, colEnd);

                ImageTransform* transform = &crop;
                transform->apply(img);

                std::cout << "\nImage cropped.\n";
                ImageIO::display(img);
            }

            else if (choice == 10) {

                ImageAnalyzer::showImageInfo(img);
            }

            else if (choice == 11) {

                ImageIO::display(img);
            }

            else {

                std::cout << ERROR_COLOR
                          << "Invalid choice. Try again.\n"
                          << RESET;
            }
        }

    }

    catch (const std::exception& e) {

        std::cout << ERROR_COLOR
                  << "Error: " << e.what() << "\n"
                  << RESET;

        return 1;
    }

    return 0;
}