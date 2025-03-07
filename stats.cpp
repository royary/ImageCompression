
#include "stats.h"
#include "compression/RGBAPixel.h"
#include <vector>

stats::stats(PNG &im)
{
  int width = im.width();
  int height = im.height();

  sumRed.resize(width, vector<long>(height));
  sumGreen.resize(width, vector<long>(height));
  sumBlue.resize(width, vector<long>(height));
  sumsqRed.resize(width, vector<long>(height));
  sumsqGreen.resize(width, vector<long>(height));
  sumsqBlue.resize(width, vector<long>(height));

  RGBAPixel *pixel;

  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      RGBAPixel *pixel = im.getPixel(x, y);
      long preSumRed = (x > 0 ? sumRed[x - 1][y] : 0) +
                       (y > 0 ? sumRed[x][y - 1] : 0) -
                       ((x > 0 && y > 0) ? sumRed[x - 1][y - 1] : 0);
      long preSumGreen = (x > 0 ? sumGreen[x - 1][y] : 0) +
                         (y > 0 ? sumGreen[x][y - 1] : 0) -
                         ((x > 0 && y > 0) ? sumGreen[x - 1][y - 1] : 0);
      long preSumBlue = (x > 0 ? sumBlue[x - 1][y] : 0) +
                        (y > 0 ? sumBlue[x][y - 1] : 0) -
                        ((x > 0 && y > 0) ? sumBlue[x - 1][y - 1] : 0);

      sumRed[x][y] = pixel->r + preSumRed;
      sumGreen[x][y] = pixel->g + preSumGreen;
      sumBlue[x][y] = pixel->b + preSumBlue;

      long preSumsqRed = (x > 0 ? sumsqRed[x - 1][y] : 0) +
                         (y > 0 ? sumsqRed[x][y - 1] : 0) -
                         ((x > 0 && y > 0) ? sumsqRed[x - 1][y - 1] : 0);
      long preSumsqGreen = (x > 0 ? sumsqGreen[x - 1][y] : 0) +
                           (y > 0 ? sumsqGreen[x][y - 1] : 0) -
                           ((x > 0 && y > 0) ? sumsqGreen[x - 1][y - 1] : 0);
      long preSumsqBlue = (x > 0 ? sumsqBlue[x - 1][y] : 0) +
                          (y > 0 ? sumsqBlue[x][y - 1] : 0) -
                          ((x > 0 && y > 0) ? sumsqBlue[x - 1][y - 1] : 0);

      sumsqRed[x][y] = (pixel->r * pixel->r) + preSumsqRed;
      sumsqGreen[x][y] = (pixel->g * pixel->g) + preSumsqGreen;
      sumsqBlue[x][y] = (pixel->b * pixel->b) + preSumsqBlue;
    }
  }
}

long stats::getSum(char channel, pair<int, int> ul, int dim)
{
  int x = ul.first;
  int y = ul.second;
  int side = pow(2, dim);
  int x2 = x + side - 1;
  int y2 = y + side - 1;
  vector<vector<long>> *sumChannel;
  // sumChannel points to one of these based on which color we're looking at
  if (channel == 'r')
  {
    sumChannel = &sumRed;
  }
  else if (channel == 'g')
  {
    sumChannel = &sumGreen;
  }
  else
  {
    sumChannel = &sumBlue;
  }

  // sumChannel2 is the actual sum calculation for the requested square region
  // It uses the cumulative sum table to calculate the sum in constant time
  long sumChannel2 = (*sumChannel)[x2][y2];
  if (x > 0)
    sumChannel2 -= (*sumChannel)[x - 1][y2];
  if (y > 0)
    sumChannel2 -= (*sumChannel)[x2][y - 1];
  if (x > 0 && y > 0)
    sumChannel2 += (*sumChannel)[x - 1][y - 1];

  return sumChannel2;
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim)
{
  int x = ul.first;
  int y = ul.second;
  int side = pow(2, dim);
  int x2 = x + side - 1;
  int y2 = y + side - 1;
  vector<vector<long>> *sumsqChannel;

  if (channel == 'r')
  {
    sumsqChannel = &sumsqRed;
  }
  else if (channel == 'g')
  {
    sumsqChannel = &sumsqGreen;
  }
  else
  {
    sumsqChannel = &sumsqBlue;
  }

  long sumsqChannel2 = (*sumsqChannel)[x2][y2];
  if (x > 0)
    sumsqChannel2 -= (*sumsqChannel)[x - 1][y2];
  if (y > 0)
    sumsqChannel2 -= (*sumsqChannel)[x2][y - 1];
  if (x > 0 && y > 0)
    sumsqChannel2 += (*sumsqChannel)[x - 1][y - 1];

  return sumsqChannel2;
}

long stats::rectArea(int dim)
{
  return pow(2, dim) * pow(2, dim);
}

// given a rectangle, compute its sum of squared deviations from mean, over all
// color channels.
double stats::getVar(pair<int, int> ul, int dim)
{
  // use the right formula
  double R = rectArea(dim);
  double sumR = getSum('r', ul, dim);
  double sumG = getSum('g', ul, dim);
  double sumB = getSum('b', ul, dim);

  double sumsqR = getSumSq('r', ul, dim);
  double sumsqG = getSumSq('g', ul, dim);
  double sumsqB = getSumSq('b', ul, dim);

  double resultR = sumsqR - (sumR * sumR) / R;
  double resultG = sumsqG - (sumG * sumG) / R;
  double resultB = sumsqB - (sumB * sumB) / R;

  return resultR + resultG + resultB;
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim)
{
  long R = rectArea(dim);
  long redSum = getSum('r', ul, dim);
  long greenSum = getSum('g', ul, dim);
  long blueSum = getSum('b', ul, dim);

  return RGBAPixel(redSum / R, greenSum / R, blueSum / R);
}
