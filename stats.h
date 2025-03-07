
#ifndef _STATS_H
#define _STATS_H

#include <cmath>
#include <utility>
#include <vector>

#include "compression/PNG.h"
#include "compression/RGBAPixel.h"
using namespace std;
using namespace compression;

class stats {
public:
    vector< vector<long> > sumRed;
    vector<vector<long> > sumGreen;
    vector< vector<long> > sumBlue;
    vector< vector<long> > sumsqRed;
    vector< vector<long> > sumsqGreen;
    vector< vector<long> > sumsqBlue;

    /** Returns the sum of all pixel values in the given colour channel in the square defined by ul and dim
     * useful in computing the average of a square
     * @param channel is one of r, g, or b
     * @param ul is (x,y) of the upper left corner of the square
     * @param dim is log of side length of the square */
    long getSum(char channel, pair<int, int> ul, int dim);

    /** Returns the sum of squares of all pixel values in the given colour channel in the square defined by ul and dim
     * useful in computing the variance of a square
     * @param channel is one of r, g, or b
     * @param ul is (x,y) of the upper left corner of the square
     * @param dim is the log of the side length of the square */
    long getSumSq(char channel, pair<int, int> ul, int dim);

    // Initialize the private vectors so that, for each color, entry
    // (x,y) is the cumulative sum of the the color values in the rectangle from (0,0)
    // to (x,y). Similarly, the sumSq vectors are the cumulative
    // sum of squares in the rectangle from (0,0) to (x,y).
    stats(PNG& im);

    /** Given a square, compute its sum of squared deviations from mean, over all color channels.
     * @param ul is (x,y) of the upper left corner of the square
     * @param dim is log of side length of the square*/
    double getVar(pair<int, int> ul, int dim);

    /** Given a square, return the average color value over the square as a pixel.
     * Each color component of the pixel is the average value of that component over
     * the square.
     * @param ul is (x,y) of the upper left corner of the square
     * @param dim is log of side length of the square */
    RGBAPixel getAvg(pair<int, int> ul, int dim);

    /** Given a square, return the number of pixels in the square
     * @param dim is log of side length of the square */
    long rectArea(int dim);
};

#endif
