#include <cv.h>  
#include <highgui.h>  
#include <stdio.h>  
  
#define max(a,b)            (((a) > (b)) ? (a) : (b))  
#define min(a,b)            (((a) < (b)) ? (a) : (b))  
  
//注意参数是有符号短整型，该函数的作用是使i限定为[a,b]区间内  
int bound(short i,short a,short b)  
{  
    return min(max(i,min(a,b)),max(a,b));  
}  
  
IplImage* src=0;  
IplImage* dst=0;  
  
void on_mouse( int event, int x, int y, int flags, void* ustc)  
{  
    char temp[16];  
    CvPoint pt;  
    CvFont font;  
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);  
    CvPoint tmp_pt = {-1,-1};  
    CvSize text_size;  
    int baseline;  
  
    if( event == CV_EVENT_MOUSEMOVE )  
    {  
        cvCopy(dst,src);      
  
        x=bound(x,0,src->width-1);  
        y=bound(y,0,src->height-1);  
        pt = cvPoint(x,y);  
        cvCircle( src, pt, 2,cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );  
  
        sprintf(temp,"(%d,%d)",x,y);  
        cvGetTextSize(temp,&font,&text_size,&baseline);  
        tmp_pt.x = bound(pt.x,0,src->width-text_size.width);  
        tmp_pt.y = bound(pt.y,text_size.height+baseline,src->height-1-baseline);  
        cvPutText(src,temp, tmp_pt, &font, cvScalar(255, 255, 255, 0));  
  
        cvShowImage( "src", src );  
    }   
    else if( event == CV_EVENT_LBUTTONDOWN )  
    {  
        //cvCopy(dst,src);  
  
        pt = cvPoint(x,y);  
        cvCircle( src, pt, 2,cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );  
  
        sprintf(temp,"(%d,%d)",x,y);  
        cvGetTextSize(temp,&font,&text_size,&baseline);  
        tmp_pt.x = bound(pt.x,0,src->width-text_size.width);  
        tmp_pt.y = bound(pt.y,text_size.height+baseline,src->height-1-baseline);  
        cvPutText(src,temp, tmp_pt, &font, cvScalar(255, 255, 255, 0));  
  
        cvCopy(src,dst);  
        cvShowImage( "src", src );  
    }   
}  
  
int main()  
{  
    src=cvLoadImage("../Lena.jpg",1);  
    dst=cvCloneImage(src);  
  
    cvNamedWindow("src",1);  
    cvSetMouseCallback( "src", on_mouse, 0 );  
      
    cvShowImage("src",src);  
    cvWaitKey(0);   
    cvDestroyAllWindows();  
    cvReleaseImage(&src);  
    cvReleaseImage(&dst);  
  
    return 0;  
}  
