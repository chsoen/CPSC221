/*
*  File: negativeColorPicker.cpp
*  Implements the negative color picker for CPSC 221 PA2.
*
*/

#include "negativeColorPicker.h"

NegativeColorPicker::NegativeColorPicker(PNG& inputimg)
{
  img = inputimg;
}

HSLAPixel NegativeColorPicker::operator()(PixelPoint p)
{
  HSLAPixel* pixel = img.getPixel(p.x, p.y);
  pixel->h = fmod((pixel->h + 180), 360);
  pixel->l = 1 - pixel->l;
  return *pixel;
}
