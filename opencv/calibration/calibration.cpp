#include <cv.h>  
#include <highgui.h>  
#include <stdio.h>  
#include <iostream>
  
#define max(a,b)            (((a) > (b)) ? (a) : (b))  
#define min(a,b)            (((a) < (b)) ? (a) : (b))  
  
using namespace std;

bool calibration = 0;
int points_count=0;
float H = 3;
float p[7][2];
float M[4][5];
IplImage* src=0;  
IplImage* dst=0;  

//注意参数是有符号短整型，该函数的作用是使i限定为[a,b]区间内  
int bound(short i,short a,short b)  
{  
    return min(max(i,min(a,b)),max(a,b));  
}  
  
  
void matrix_multi(float a[4][5], float b[5], float ans[4])
{
    ans[1] = 0;
    ans[2] = 0;
    ans[3] = 0;
    for (int i=1; i<=3; i++)
    {
        for (int j=1; j<=4; j++)
        {
            ans[i] += a[i][j]*b[j];
        }
    }
}

void camera_calibration()
{
    M[3][4] = H;
    M[1][4] = p[1][0] * H; 
    M[2][4] = p[1][1] * H;
    M[3][3] = ((p[1][0]-p[2][0])*(p[3][1]-p[4][1])-(p[3][0]-p[4][0])*(p[1][1]-p[2][1]))/((p[2][0]-p[4][0])*(p[3][1]-p[4][1])-(p[3][0]-p[4][0])*(p[2][1]-p[4][1]));
    M[3][2] = ((p[1][0]-p[3][0])*(p[2][1]-p[4][1])-(p[2][0]-p[4][0])*(p[1][1]-p[3][1]))/((p[3][0]-p[4][0])*(p[2][1]-p[4][1])-(p[2][0]-p[4][0])*(p[3][1]-p[4][1]));
    M[1][3] = (p[1][0]*p[3][0]*(p[2][1]-p[4][1])+p[1][0]*p[4][0]*(p[3][1]-p[2][1])+p[2][0]*p[3][0]*(p[4][1]-p[1][1])+p[2][0]*p[4][0]*(p[1][1]-p[3][1]))/((p[2][0]-p[4][0])*(p[3][1]-p[4][1])-(p[3][0]-p[4][0])*(p[2][1]-p[4][1]));
    M[1][2] = (p[1][0]*p[2][0]*(p[3][1]-p[4][1])+p[1][0]*p[4][0]*(p[2][1]-p[3][1])+p[2][0]*p[3][0]*(p[4][1]-p[1][1])+p[3][0]*p[4][0]*(p[1][1]-p[2][1]))/((p[3][0]-p[4][0])*(p[2][1]-p[4][1])-(p[2][0]-p[4][0])*(p[3][1]-p[4][1]));
    M[2][3] = ((p[1][0]*p[2][1]-p[2][0]*p[1][1])*(p[3][1]-p[4][1])+(p[4][0]*p[3][1]-p[3][0]*p[4][1])*(p[1][1]-p[2][1]))/((p[2][0]-p[4][0])*(p[3][1]-p[4][1])-(p[3][0]-p[4][0])*(p[2][1]-p[4][1]));
    M[2][2] = ((p[1][0]*p[3][1]-p[3][0]*p[1][1])*(p[2][1]-p[4][1])+(p[4][0]*p[2][1]-p[2][0]*p[4][1])*(p[1][1]-p[3][1]))/((p[3][0]-p[4][0])*(p[2][1]-p[4][1])-(p[2][0]-p[4][0])*(p[3][1]-p[4][1]));
    M[3][1] = (p[1][1]-p[2][1]-p[5][1]+p[6][1]-(p[2][1]-p[6][1])*M[3][3])/(p[5][1]-p[6][1]);
    M[1][1] = p[5][0]-p[1][0]+p[5][0]*M[3][1];
    M[2][1] = p[5][1]-p[1][1]+p[5][1]*M[3][1];

    cout << "Metrix M" << endl;
    for (int i=1; i<=3; i++)
    {
        for (int j=1; j<=4; j++)
        {
            cout << M[i][j] << ",";
        }
        cout << endl;
    }
    calibration = 1;
}

void on_mouse( int event, int x, int y, int flags, void* ustc)  
{  
    char temp[16];  
    CvPoint pt;  
    CvFont font;  
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);  
    CvPoint tmp_pt = {-1,-1};  
    CvSize text_size;  
    int baseline;  
  
    if( !calibration && event == CV_EVENT_MOUSEMOVE )  
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
    else if( !calibration && event == CV_EVENT_LBUTTONDOWN )  
    {  
        //cvCopy(dst,src);  
        points_count++; 
        cout << "count: " << points_count << endl;
        p[points_count][0] = x;
        p[points_count][1] = y; 
        if ( points_count == 6 ) 
            camera_calibration();

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
    src=cvLoadImage("cube.jpg",1);  
    dst=cvCloneImage(src);  
  
    cvNamedWindow("src",1);  
    cvSetMouseCallback( "src", on_mouse, 0 );  
      
    cvShowImage("src",src);  

    cvWaitKey(0);   

    while ( calibration )
    { 
        char temp[64];  
        CvPoint pt;  
        CvFont font;  
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);  
        CvPoint tmp_pt = {-1,-1};  
        CvSize text_size;  
        int baseline;  
        
        float X, Y, Z;
        float V[5];
        float ans[4];

        cout << "Input X, Y, Z" << endl;
        cin >> X >> Y >> Z;

        V[1] = X;
        V[2] = Y;
        V[3] = Z;
        V[4] = 1; 
        
        matrix_multi(M, V, ans);
        pt = cvPoint(ans[1]/ans[3], ans[2]/ans[3]);
        cvCircle( src, pt, 2, cvScalar(0, 255, 0, 0), CV_FILLED, CV_AA, 0);

        cout << ans[1]/ans[3] << "," << ans[2]/ans[3] << endl;
        sprintf(temp, "(%.2f, %.2f, %.2f)", V[1]/V[4], V[2]/V[4], V[3]/V[4]);
        cout << temp << endl;
        cvGetTextSize(temp, &font, &text_size, &baseline);
        tmp_pt.x = bound(pt.x, 0, src->width-text_size.width);
        tmp_pt.y = bound(pt.y, text_size.height+baseline, src->height-1-baseline);
        cvPutText(src, temp, tmp_pt, &font, cvScalar(255, 255, 255, 0));

        cvCopy(src, dst);
        cvShowImage("src", src);
        cvWaitKey(0);
    }
    cvDestroyAllWindows();  
    cvReleaseImage(&src);  
    cvReleaseImage(&dst);  
  
    return 0;  
}  
