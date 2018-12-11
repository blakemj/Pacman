#include "gl_more.h"
#include "gl.h"

/*
* This function will take in a rectangle with top left corner at coordinate (xCoord, yCoord) with width and height. This will
* loop through all of the pixels in this rectangle and determine whether they fall inside a circle with a diameter of the width.
* This will then fill in the pixels that are inside with color.
*/
void gl_draw_circle(int xCoord, int yCoord, int width, int height, color_t color) {
   double xCenter = xCoord + (width / 2);
   double yCenter = yCoord + (height / 2);
   for (int i = 1; i < width; i++) {
       for (int j = 1; j < height; j++) {
           if ((xCoord + i - xCenter) * (xCoord + i - xCenter) + (yCoord + j - yCenter) * (yCoord + j - yCenter) <=(width / 2) * (width / 2)) gl_draw_pixel(xCoord + i, yCoord + j, color);
       }
   }
}
