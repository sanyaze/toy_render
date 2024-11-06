#include "math_tools.h"
#include "log_debug.h"
#include "bmp_creater.h"
#include <fstream>

int main()
{
    int width = 500;
    int height = 500;
    BmpCreater bmp(width, height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i*i + j*j <= 500*500) {
                bmp.SetPixel(i, j, 255, 0, 0);
            }
        }
    }
    
    bmp.Save("test.bmp");
    return 0;
}