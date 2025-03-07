/**
 * @file PNG.h
 *
 * @author CS 221: Data Structures
 */

#ifndef COMPRESSION_PNG_H
#define COMPRESSION_PNG_H

#include <string>
#include <vector>

#include "RGBAPixel.h"

using namespace std;

namespace compression {
    class PNG {
    public:
        /**
         * Creates an empty PNG image.
         */
        PNG();

        /**
         * Creates a PNG image of the specified dimensions.
         * @param width Width of the new image.
         * @param height Height of the new image.
         */
        PNG(unsigned int width, unsigned int height);

        /**
         * Copy constructor: creates a new PNG image that is a copy of
         * another.
         * @param other PNG to be copied.
         */
        PNG(const PNG& other);

        /**
         * Destructor: frees all memory associated with a given PNG object.
         * Invoked by the system.
         */
        ~PNG();

        /**
         * Assignment operator for setting two PNGs equal to one another.
         * @param other Image to copy into the current image.
         * @return The current image for assignment chaining.
         */
        const PNG& operator=(const PNG& other);

        /**
         * Equality operator: checks if two images are the same.
         * @param other Image to be checked.
         * @return Whether the current image is equal to the other image.
         */
        bool operator==(const PNG& other) const;

        /**
         * Inequality operator: checks if two images are different.
         * @param other Image to be checked.
         * @return Whether the current image differs from the other image.
         */
        bool operator!=(const PNG& other) const;

        /**
         * Reads in a PNG image from a file.
         * Overwrites any current image content in the PNG.
         * @param fileName Name of the file to be read from.
         * @return true, if the image was successfully read and loaded.
         */
        bool readFromFile(const string& fileName);

        /**
         * Writes a PNG image to a file.
         * @param fileName Name of the file to be written.
         * @return true, if the image was successfully written.
         */
        bool writeToFile(const string& fileName);

        /**
         * Pixel access operator. Gets a pointer to the pixel at the given
         * coordinates in the image. (0,0) is the upper left corner.
         * This pointer allows the image to be changed.
         * @param x X-coordinate for the pixel pointer to be grabbed from.
         * @param y Y-coordinate for the pixel pointer to be grabbed from.
         * @return A pointer to the pixel at the given coordinates.
         */
        RGBAPixel* getPixel(unsigned int x, unsigned int y);

        /**
         * Gets the width of this image.
         * @return Width of the image.
         */
        unsigned int width() const;

        /**
         * Gets the height of this image.
         * @return Height of the image.
         */
        unsigned int height() const;

        /**
         * Resizes the image to the given coordinates. Attempts to preserve
         * existing pixel data in the image when doing so, but will crop if
         * necessary. No pixel interpolation is done.
         * @param newWidth New width of the image.
         * @param newHeight New height of the image.
         */
        void resize(unsigned int newWidth, unsigned int newHeight);

    private:
        unsigned int width_;   /*< Width of the image */
        unsigned int height_;  /*< Height of the image */
        RGBAPixel* imageData_; /*< Array of pixels */

        /**
         * Copeies the contents of `other` to self
         */
        void _copy(const PNG& other);
    };
}

#endif
