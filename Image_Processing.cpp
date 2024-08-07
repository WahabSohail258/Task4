#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Photoshop {
private:
    Mat img;
    Mat original;

public:
    Photoshop(const string& imagePath) {
        original = imread(imagePath);
        img = original.clone();
    }

    ~Photoshop() {}

    void displayImage() {
        imshow("Current Image", img);
        waitKey(2000);
    }

    void applyFilter(int filterChoice) {
        Mat imgFiltered;

        switch (filterChoice) {
        case 1: {
            cvtColor(img, imgFiltered, COLOR_BGR2GRAY);
            break;
        }

        case 2: {
            GaussianBlur(img, imgFiltered, Size(7, 7), 5, 0);
            break;
        }

        case 3: {
            Canny(img, imgFiltered, 50, 150);
            break;
        }

        case 4: {
            Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
            filter2D(img, imgFiltered, img.depth(), kernel);
            break;
        }

        default: {
            cout << "Invalid filter choice\n";
            return;
        }
        }

        img = imgFiltered.clone();
        displayImage();
    }

    void resizeImage(int width, int height) {
        Mat imgResized;
        resize(img, imgResized, Size(width, height));
        img = imgResized.clone();
        displayImage();
    }

    void rotateImage(int angle) {
        Mat imgRotated;
        Point2f center(img.cols / 2.0, img.rows / 2.0);
        Mat rotationMatrix = getRotationMatrix2D(center, angle, 1.0);
        warpAffine(img, imgRotated, rotationMatrix, img.size());
        img = imgRotated.clone();
        displayImage();
    }

    void adjustBrightnessContrast(double con, double bri) {
        Mat imgAdjusted;
        img.convertTo(imgAdjusted, -1, con, bri);
        img = imgAdjusted.clone();
        displayImage();
    }

    void cropImage(int x, int y, int width, int height) {
        Rect roi(x, y, width, height);
        Mat croppedImage = img(roi).clone();
        img = croppedImage.clone();
        displayImage();
    }

    void saveImage(const string& filename) {
        imwrite(filename, img);
        cout << "Image saved as " << filename << endl;
    }

    void resetAllChanges() {
        img = original.clone();
        displayImage();
    }
};

int main() {
    Photoshop ps("lena.png");

    char key;

    do {
        int choice;
        cout << "Enter your choice:\n0. Display Current Image\n1. Apply Filter\n2. Resize Image\n3. Rotate Image\n4. Adjust Brightness and Contrast\n5. Crop Image\n6. Save Image\n7. Reset All Changes\nPress 'q' to quit\n";
        cin >> choice;

        switch (choice) {
        case 0: {
            ps.displayImage();
            break;
        }

        case 1: {
            int filterChoice;
            cout << "Choose a filter:\n1. Grayscale\n2. Gaussian Blur\n3. Canny Edge Detection\n4. Sharpen\n";
            cin >> filterChoice;
            ps.applyFilter(filterChoice);
            break;
        }

        case 2: {
            int width, height;
            cout << "Enter the width of the image:";
            cin >> width;
            cout << "Enter the height of the image:";
            cin >> height;
            ps.resizeImage(width, height);
            break;
        }

        case 3: {
            int angle;
            cout << "Enter the angle for the rotation of the image:";
            cin >> angle;
            ps.rotateImage(angle);
            break;
        }

        case 4: {
            double contrast, brightness;
            cout << "Enter the contrast value: ";
            cin >> contrast;
            cout << "Enter the brightness value: ";
            cin >> brightness;
            ps.adjustBrightnessContrast(contrast, brightness);
            break;
        }

        case 5: {
            int x, y, width, height;
            cout << "Enter the x-coordinate of the top-left corner of the ROI: ";
            cin >> x;
            cout << "Enter the y-coordinate of the top-left corner of the ROI: ";
            cin >> y;
            cout << "Enter the width of the ROI: ";
            cin >> width;
            cout << "Enter the height of the ROI: ";
            cin >> height;
            ps.cropImage(x, y, width, height);
            break;
        }

        case 6: {
            string filename;
            cout << "Enter the filename to save the image (including extension): ";
            cin >> filename;
            ps.saveImage(filename);
            break;
        }

        case 7: {
            ps.resetAllChanges();
            break;
        }

        case 'q': {
            cout << "Quitting...\n";
            return 0;
        }

        default: {
            cout << "Invalid choice\n";
            break;
        }
        }

        cout << "Press 'q' to quit or any other key to continue\n";
        cin >> key;     
    } while (key != 'q');

    return 0;
}
