/*
*  File: censorColorPicker.cpp
*  Implements the censor color picker for CPSC 221 PA2.
*
*/

#include "censorColorPicker.h"

#include <math.h> // gives access to sqrt function

/*
*  Useful function for computing the Euclidean distance between two PixelPoints
*/
double PointDistance(PixelPoint a, PixelPoint b) {
  unsigned int diff_x, diff_y;
  if (a.x > b.x)
    diff_x = a.x - b.x;
  else
    diff_x = b.x - a.x;
  if (a.y > b.y)
    diff_y = a.y - b.y;
  else
    diff_y = b.y - a.y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}

CensorColorPicker::CensorColorPicker(unsigned int b_width, PixelPoint ctr, unsigned int rad, PNG& inputimage)
{
  blockwidth = b_width;
  center = ctr;
  radius = rad;
  img = inputimage;
  blockyimg.resize(ceil(img.width() / (double) blockwidth), ceil(img.height() / (double) blockwidth));

  for (double cornerx=0; cornerx<img.width(); cornerx+=blockwidth) {
    for (double cornery=0; cornery<img.height(); cornery+=blockwidth) {
      double sumH = 0;
      double sumS = 0;
      double sumL = 0;
      double sumA = 0;

      int totalPixels = 0;
      for (unsigned int x=0; x<blockwidth; x++) {
        if (cornerx+x >= img.width()) {
          break; // if x runs off image, break loop
        }
        for (unsigned int y=0; y<blockwidth; y++) {
          if (cornery+y >= img.height()) {
            break; // if y runs off image, break loop
          }
          // taking the sum of each hsla field in block
          HSLAPixel* pixel = img.getPixel(cornerx+x, cornery+y);
          sumH += pixel->h;
          sumS += pixel->s;
          sumL += pixel->l;
          sumA += pixel->a;
          totalPixels++;
        }
      }
      
      HSLAPixel average = HSLAPixel(sumH / totalPixels, sumS / totalPixels, sumL / totalPixels, sumA / totalPixels);
      *blockyimg.getPixel(floor(cornerx / blockwidth), floor(cornery / blockwidth)) = average;
    }
  }
}

HSLAPixel CensorColorPicker::operator()(PixelPoint p)
{
  HSLAPixel* pixel;
  if (PointDistance(p, center) <= radius) {
    pixel = blockyimg.getPixel(floor(p.x / (double) blockwidth), floor(p.y / (double) blockwidth));
  } else {
    pixel = img.getPixel(p.x, p.y);
  }
  return *pixel;
}
