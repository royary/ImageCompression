/**
 * @file PNG.cpp
 * Implementation of a simple PNG class using RGBAPixels and the lodepng PNG library.
 *
 * 
 */

#include "PNG.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "lodepng/lodepng.h"

namespace compression {
    void PNG::_copy(const PNG& other) {
        // Clear self
        delete[] imageData_;

        // Copy `other` to self
        width_ = other.width_;
        height_ = other.height_;
        imageData_ = new RGBAPixel[width_ * height_];
        //we conceptually think of an image as a 2D grid, it's actually stored as a 1D array
        for (unsigned i = 0; i < width_ * height_; i++) {
            imageData_[i] = other.imageData_[i];
        }
    }

    PNG::PNG() {
        width_ = 0;
        height_ = 0;
        imageData_ = nullptr;
    }

    PNG::PNG(unsigned int width, unsigned int height) {
        width_ = width;
        height_ = height;
        imageData_ = new RGBAPixel[width * height];
    }

    PNG::PNG(const PNG& other) {
        imageData_ = nullptr;
        _copy(other);
    }

    PNG::~PNG() {
        delete[] imageData_;
    }

    const PNG& PNG::operator=(const PNG& other) {
        if (this != &other) { _copy(other); }
        return *this;
    }

    bool PNG::operator==(const PNG& other) const {
        if (width_ != other.width_) { return false; }
        if (height_ != other.height_) { return false; }

        for (unsigned i = 0; i < width_ * height_; i++) {
            RGBAPixel& p1 = imageData_[i];
            RGBAPixel& p2 = other.imageData_[i];
            if (p1 != p2) { return false; }
        }

        return true;
    }

    bool PNG::operator!=(const PNG& other) const {
        return !(*this == other);
    }

    RGBAPixel* PNG::getPixel(unsigned int x, unsigned int y) {
        if (width_ == 0 || height_ == 0) {
            cerr << "ERROR: Call to compression::PNG::getPixel() made on an image with no pixels." << endl;
            cerr << "     : Returning nullptr." << endl;
            return nullptr;
        }

        if (x >= width_) {
            cerr << "WARNING: Call to compression::PNG::getPixel(" << x << "," << y << ") tries to access x=" << x
                 << ", which is outside of the image (image width: " << width_ << ")." << endl;
            cerr << "       : Truncating x to " << (width_ - 1) << endl;
            x = width_ - 1;
        }

        if (y >= height_) {
            cerr << "WARNING: Call to compression::PNG::getPixel(" << x << "," << y << ") tries to access y=" << y
                 << ", which is outside of the image (image height: " << height_ << ")." << endl;
            cerr << "       : Truncating y to " << (height_ - 1) << endl;
            y = height_ - 1;
        }

        unsigned index = x + (y * width_);
        return imageData_ + index;
    }

    bool PNG::readFromFile(const string& fileName) {
        vector<unsigned char> byteData;
        unsigned error = lodepng::decode(byteData, width_, height_, fileName);

        if (error) {
            cerr << "PNG decoder error " << error << ": " << lodepng_error_text(error) << endl;
            return false;
        }

        delete[] imageData_;
        imageData_ = new RGBAPixel[width_ * height_];

        for (unsigned i = 0; i < byteData.size(); i += 4) {
            RGBAPixel& pixel = imageData_[i / 4];
            pixel.r = byteData[i];
            pixel.g = byteData[i + 1];
            pixel.b = byteData[i + 2];
            pixel.a = byteData[i + 3] / 255.;
        }

        return true;
    }

    bool PNG::writeToFile(const string& fileName) {
        unsigned char* byteData = new unsigned char[width_ * height_ * 4];

        for (unsigned i = 0; i < width_ * height_; i++) {
            byteData[(i * 4)] = imageData_[i].r;
            byteData[(i * 4) + 1] = imageData_[i].g;
            byteData[(i * 4) + 2] = imageData_[i].b;
            byteData[(i * 4) + 3] = imageData_[i].a * 255;
        }

        unsigned error = lodepng::encode(fileName, byteData, width_, height_);
        if (error) {
            cerr << "PNG encoding error " << error << ": " << lodepng_error_text(error) << endl;
        }

        delete[] byteData;
        return (error == 0);
    }

    unsigned int PNG::width() const {
        return width_;
    }

    unsigned int PNG::height() const {
        return height_;
    }

    void PNG::resize(unsigned int newWidth, unsigned int newHeight) {
        // Create a new vector to store the image data for the new (resized) image
        RGBAPixel* newImageData = new RGBAPixel[newWidth * newHeight];

        // Copy the current data to the new image data, using the existing pixel
        // for coordinates within the bounds of the old image size
        for (unsigned x = 0; x < newWidth; x++) {
            for (unsigned y = 0; y < newHeight; y++) {
                if (x < width_ && y < height_) {
                    RGBAPixel* oldPixel = this->getPixel(x, y);
                    RGBAPixel& newPixel = newImageData[(x + (y * newWidth))];
                    newPixel = *oldPixel;
                }
            }
        }

        // Clear the existing image
        delete[] imageData_;

        // Update the image to reflect the new image size and data
        width_ = newWidth;
        height_ = newHeight;
        imageData_ = newImageData;
    }
}
