/**
 * @file RGBAPixel.h
 */

#ifndef _RGBAPIXEL_H_
#define _RGBAPIXEL_H_

namespace compression {
    class RGBAPixel {
    public:               // member attributes
        unsigned char r;  // 8-bit red channel, integer in the range of [0,255]
        unsigned char g;  // 8-bit green channel, integer in the range of [0,255]
        unsigned char b;  // 8-bit blue channel, integer in the range of [0,255]
        double a;         // alpha channel, double in the range of [0,1]. 0: transparent, 1: opaque

    public:  // member functions
        // (in)equality test operators
        bool operator==(RGBAPixel const& other) const;
        bool operator!=(RGBAPixel const& other) const;

        /**
         * Constructs a default RGBAPixel. A default pixel is completely
         * opaque (non-transparent) and white. Opaque implies that the
         * alpha component of the pixel is 1.0. Lower values are
         * transparent.
         */
        RGBAPixel();

        /**
         * Constructs an opaque RGBAPixel with the given red, green, and blue
         * channel values. The alpha component of the pixel constructed
         * should be 1.0.
         * @param red Value for the red channel, [0, 255].
         * @param green Value for the green channel, [0, 255].
         * @param blue Value for the blue channel, [0, 255].
         */
        RGBAPixel(int red, int green, int blue);

        /**
         * Constructs an opaque RGBAPixel with the given red, green,
         * blue, and alpha values.
         * @param red Value for the red channel, [0, 255].
         * @param green Value for the green channel, [0, 255].
         * @param blue Value for the blue channel, [0, 255].
         * @param alpha Alpha value for the new pixel, [0, 1].
         */
        RGBAPixel(int red, int green, int blue, double alpha);
    };
}
#endif