#include "gl_more.h"
#include "gl.h"

void gl_draw_circle(int xCoord, int yCoord, int width, int height, color_t color) {
   double xCenter = xCoord + (width / 2);
   double yCenter = yCoord + (height / 2);
   for (int i = 0; i < width; i++) {
       for (int j = 0; j < height; j++) {
           if ((xCoord + i - xCenter) * (xCoord + i - xCenter) + (yCoord + j - yCenter) * (yCoord + j - yCenter) <=(width / 2) * (width / 2)) gl_draw_pixel(xCoord + i, yCoord + j, color);
       }
   }
}
