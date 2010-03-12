#include "cv.h"
#include "highgui.h"
#include <signal.h>
#include <iostream>
#include <sstream>

bool run = true;

void sighandler ( int sig ) {
	run = false;
}

int main( int argc, char * argv[] ) {


	signal( SIGABRT, &sighandler );
	signal( SIGTERM, &sighandler );
	signal( SIGINT, &sighandler );

	CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
	if( ! capture ) {
		std::cerr << "ERROR: Failed to start capture device." << std::endl;
		return -1;
	}

	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );

	//cvNamedWindow( "view", CV_WINDOW_AUTOSIZE );

	IplImage * frame;
	CvScalar pixel;
	int i,j,k;
	bool sentinel;

	k = 0;
	while( run ) {
		k++;
		sentinel = false;
		frame = cvQueryFrame( capture );

		std::cout << "== Frame: " << k << " ==" << std::endl;

		if( ! frame ) {
			std::cerr << "ERROR: Couldn't get a frame!" << std::endl;
			break;
		}

		//cvShowImage( "view", frame );

		for( i = 0; i < frame->height; i++ ) {
			for( j = 0; j < frame->width; j++ ) {
				pixel = cvGet2D( frame, i, j );
				if( pixel.val[0] > 200 ) {
					std::cout << "Row: " << i << " Column: " << j << " Data: ( " << pixel.val[0] << ", " << pixel.val[1] << ", " << pixel.val[2] << " )" << std::endl;
					sentinel = true;
					break;
				}
			}
			if( sentinel ) { break; }
		}

	}

	std::cout << "Shutting down..." << std::endl;

	cvReleaseCapture( &capture );
	//cvDestroyWindow( "view" );

	return 0;
}