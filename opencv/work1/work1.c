#include "cv.h"
#include "highgui.h"

int main(int argc, char** argv)
{
    IplImage* pImg;

    if ( argc == 2 &&
            (pImg = cvLoadImage( argv[1], 1)) != 0)
    {
        cvNamedWindow("Image", 1);
        cvShowImage("Image", pImg);

        cvWaitKey(0);

        cvDestroyWindow("Image");
        cvReleaseImage(&pImg);
        return 0;
    }

    return -1;
}
