#include "cv.h"
#include "highgui.h"
#include <signal.h>
#include <iostream>
#include <sstream>
#include <fstream>

bool run = true;

void sighandler ( int sig ) {
	run = false;
}

int main( int argc, char ** argv ) {

	char mask = (char) 1;
	char buffer = (char) 0;
	int counter = 0;
	if( 1 != sizeof( mask ) ) {
		std::cerr << "Uh oh! Your char size on this arch isn't a single byte. I can't fit that into my worldview." << std::endl;
		return -1;
	}
	
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

	std::ofstream file( "data.bin", std::ios::out | std::ios::binary | std::ios::app );
	
	int block_index = file.tellp();
	
	while( run ) {
		sentinel = false;
		frame = cvQueryFrame( capture );

		if( ! frame ) {
			std::cerr << "ERROR: Couldn't get a frame!" << std::endl;
			break;
		}

		//cvShowImage( "view", frame );

		for( i = 0; i < frame->height; i++ ) {
			for( j = 0; j < frame->width; j++ ) {
				pixel = cvGet2D( frame, i, j );
				if( pixel.val[0] > 200 ) {
					int x = ( i * frame->width ) + j;
					buffer = buffer << 1;
					buffer = buffer ^ ( ( buffer ^ (char) x ) & mask );
					if( ++counter >= 8 ) {
						file.write( &buffer, 1 );
						file.flush();
						++block_index;
						if( block_index >= 32 ) {
							std::cout << "Captured 32 bytes total. Exiting." << std::endl;
							run = false;
						}
						buffer = (char) 0;
						counter = 0;
					}
					sentinel = true;
					break;
				}
			}
			if( sentinel ) { break; }
		}

	}

	std::cout << "Shutting down..." << std::endl;

	file.close();
	
	cvReleaseCapture( &capture );
	//cvDestroyWindow( "view" );

	return 0;
}