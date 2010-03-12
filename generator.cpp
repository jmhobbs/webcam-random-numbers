#include "cv.h"
#include "highgui.h"
#include <signal.h>
#include <iostream>
#include <fstream>

// Totally arbitrary...
#define BLOCK_SIZE 32
// Almost arbitrary...
#define THRESHOLD 215
// I visually observed that the largest scintillations only affected a 4x4 block of pixels. So, skipping 4 rows seems safe FOR MY CAMERA
#define ROW_SKIP 4

bool run = true;

void sighandler ( int sig ) {
	run = false;
}

int main( int argc, char ** argv ) {

	char mask = (char) 1;
	char buffer = (char) 0;
	int counter = 0;
	if( 1 != sizeof( mask ) ) {
		std::cerr << "Uh oh! Your char size on this arch isn't a single byte. I can't fit that into my world view." << std::endl;
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

	IplImage * frame;
	
	// On my camera, the first few frames are junked, so I scrap them here.
	for( int x = 0; x <= 3; ++x )
		frame = cvQueryFrame( capture );
	
	CvScalar pixel;
	int row,col;

	std::ofstream file( "data.bin", std::ios::out | std::ios::binary | std::ios::app );
	
	int block_index = file.tellp();
	bool block_image_captured = ( 0 != block_index );
	
	while( run ) {

		frame = cvQueryFrame( capture );

		if( ! frame ) {
			std::cerr << "ERROR: Couldn't get a frame!" << std::endl;
			break;
		}

		if( ! block_image_captured ) {
			if( cvSaveImage( "data.png", frame ) ) {
				std::cout << "Saved an image for the block." << std::endl;
				block_image_captured = true;
			}
		}

		for( row = 0; row < frame->height; row++ ) {
			for( col = 0; col < frame->width; col++ ) {
				pixel = cvGet2D( frame, row, col );
				if( pixel.val[0] > THRESHOLD && pixel.val[1] > THRESHOLD && pixel.val[2] > THRESHOLD ) {
					
					// Shift the LSB of the index onto the buffer
					int index = ( row * frame->width ) + col;
					buffer = buffer << 1;
					buffer = buffer ^ ( ( buffer ^ (char) index ) & mask );
					
					// Full buffer?
					if( ++counter >= 8 ) {
					
						// Write it to file immediately
						file.write( &buffer, 1 );
						file.flush();
						
						// Full block?
						if( ++block_index >= BLOCK_SIZE ) {
							std::cout << "Captured a full block. Exiting." << std::endl;
							run = false;
							row = frame->height;
							break;
						}
						
						// Reset our buffer
						buffer = (char) 0;
						counter = 0;
					}
					// Skip a few rows to avoid double registering large scintillations
					row += ROW_SKIP;
					break;
				}
			}
		}

	}

	std::cout << "Shutting down..." << std::endl;

	file.close();
	
	cvReleaseCapture( &capture );

	return 0;
}
