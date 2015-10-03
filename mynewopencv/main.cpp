#include<stdlib.h>
#include<stdio.h>
#include<iostream>
//#include<MMSystem.h>
#include<windows.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
//using namespace std;


int main()
{ 
	//freopen ("myfile.txt","w",stdout);
	IplImage* background=NULL; //intel image processing libraray
	IplImage* foreground=NULL; 
	IplImage* subtract=NULL; 
	IplConvKernel* element=NULL;
	CvCapture* capture=NULL; //info for reading frames from a camera or a video file
	int counter=0;	
	//show image on screen

	//cvNamedWindow("Video",CV_WINDOW_AUTOSIZE); //CV_WINDOW_AUTOSIZE is the default video size
	cvNamedWindow("Subtract",CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("Contour",CV_WINDOW_AUTOSIZE);
	IplImage* background_gry ;
	IplImage* foreground_gry;
	IplImage* foreground_skipped;
	IplImage* eroded;
	IplImage* dilated;
	IplImage* frame;
	IplImage* audio;

	CvRect rect;
	CvSeq* contours=0;
	CvMemStorage *storage = cvCreateMemStorage(0);
	//float ar;
	//capture = cvCaptureFromCAM(0);
	capture = cvCaptureFromFile("4.avi"); //takes camera ID as argument

	/* if( !capture ) 
	{ 
	printf("Cannot open camera\n"); 
	return -1; 
	}*/
	if(!background) 
	{ 
		for(int i=0;i<50;i++)
		{ 
			background=cvQueryFrame(capture); 
			//takes as its argument a pointer to CvCapture structure
			//grabs next video frame into memory		


			background_gry = cvCreateImage(cvGetSize(background), 8,1); 
			// parameters are size, depth and channels
			//size is image height and width
			//depth - bit depth of image elements
			//channel - no of channels per pixel, supports 1-4 channels
			eroded = cvCreateImage(cvGetSize(background), 8, 1);
			dilated = cvCreateImage(cvGetSize(background), 8, 1);
			frame = cvCreateImage(cvGetSize(background), 8, 1);
			cvCvtColor(background, background_gry, CV_RGB2GRAY); 
			// converts one color space(no of channels) to another
			//parameters - input image, output image, color space conversion code
		} 

	} 
	cvWaitKey(150); //wait and stop for a key stroke
	subtract= cvCreateImage( cvGetSize(background), 8, 1);
	IplImage* subtract_gry = cvCreateImage( cvGetSize(background), 8, 1);
	IplImage* subtract_final = cvCreateImage( cvGetSize(background), 8, 1);

while(1) 
{ 
	foreground=cvQueryFrame(capture); 
	//if( !foreground ) break; 
	IplImage* foreground_skipped = cvCreateImage( cvGetSize(foreground), 8, 1);

	if((counter%5)==0)
	{	
		foreground_skipped = foreground;
		//cvNamedWindow("frame", CV_WINDOW_AUTOSIZE);
		//cvShowImage("frame", foreground);
		//cvWaitKey(100);

		foreground_gry= cvCreateImage(cvGetSize(foreground), 8,1);
		cvCvtColor(foreground, foreground_gry, CV_RGB2GRAY);
		//cvShowImage( "Video",foreground_gry); 
		char c = cvWaitKey(30); 
		// for(int j=0;c!=27;j++){ 
		//cvSub(foreground,background,subtract,NULL); 

		cvAbsDiff(foreground_gry,background_gry,subtract); 
		//cvCvtColor(subtract, subtract_gry, CV_RGB2GRAY);
		background_gry = foreground_gry;
		//cvShowImage( "Background", background_gry );
		//double sub_pix= cvGetReal2D(subtract_gry, 150, 100);
		//printf("\n sub= %lf", sub_pix);
		cvThreshold(subtract, subtract_final, 15.0, 255.0, CV_THRESH_BINARY);
		cvErode(subtract_final, eroded, element, 3);
		cvDilate(eroded, dilated, element, 40);

		//defining color for drawing contours
		CvScalar(ext_color);
		ext_color = CV_RGB(rand()&255, rand()&255, rand()&255);
		//finding contours for the subtracted image 
		cvFindContours(dilated, storage, &contours, sizeof(CvContour),CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

		//drawing the contours
//		for (; contours != 0; contours = contours->h_next)
		if (contours != 0)
		{
			cvDrawContours(dilated, contours, ext_color, CV_RGB(4,2,1), -1, 2, 8);
			rect = cvBoundingRect(contours, 0); //extract bounding box for current contour
			//drawing rectangle

			if( (rect.height * rect.width) < 10000)
			{
				contours = contours->h_next;
			}	
			else
			{
				cvRectangle(background, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y+rect.height), cvScalar(0, 50, 255, 0), 10, 4, 0); 
				//float ar = rect.x/rect.y;	
				float ar = (rect.width-rect.x)/(rect.height-rect.y);
				printf("\n%2f",ar);
				printf("\t\t");

				//float er = 786432/(((rect.x+rect.width))*((rect.y+rect.height)));
				//printf("%2f\n",er);
				//printf("\t\t\t");


				float w,v,y,d;
				float dif=200,cvr;
				v=0.5;
				d = pow(rect.width,2.0)+pow(rect.height,2.0);
				w=pow(d,v);
				y=w/2;
				
				cvr=dif-y;
				printf("%f\t\t%f\n",y,cvr);
				dif = y;


				for(int i=0;i<3;i++)
				{
					if(ar<-15)
				  	{
						if(cvr<-75)
						{
							//if(rect.width<rect.height)
							//{
								printf("Fall detected \n");
								
								//system("CSmtp.exe");
								
								//PlaySound(TEXT("ring.avi"), NULL, SND_SYNC);
								
								
							//}
						}
					}
				}
			}
		
		//cvBoundingRect(contours,0);
		cvShowImage("Subtract",background); 
		
		//cvShowImage("video",background);
		cvWaitKey(2);
		// } 
		if( c == 27 ) break; 
	}
	}
counter++;
}

cvReleaseCapture( &capture ); 
//cvDestroyWindow("Background"); 
cvDestroyWindow("Video"); 
cvDestroyWindow("Subtract"); 
cvReleaseMemStorage(&storage);
return 0;
}
