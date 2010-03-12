#include "cv.h"
#include "highgui.h"
#include <signal.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>

// Totally arbitrary.
#define SAMPLE_SIZE 128
// Almost arbitrary...
#define THRESHOLD 215
// I visually observed that the largest scintillations only affected a 4x4 block of pixels. So, skipping 4 rows seems safe FOR MY CAMERA
// Figure out your own row skip you lazy bum.
#define ROW_SKIP 4
// How consistent is your camera? Shielding might help ;-)
#define MAX_CONSECUTIVE_DROPS 10
// Show a progress bar every X bytes
#define SAMPLE_PROGRESS_INTERVAL 12

bool run = true;

std::string get_sample_name () {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime( buffer, 80, "data.%Y%m%d.%H%M%S", timeinfo );

	return std::string ( buffer );
}

std::string get_time_stamp () {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime( buffer, 80, "[ %Y-%m-%d %H:%M:%S ]   ", timeinfo );

	return std::string ( buffer );
}

std::string get_progress_bar ( int complete ) {
	
	int pct = floor( (float) complete / (float) SAMPLE_SIZE * 100 );
	
	std::stringstream utl;
	utl << "[";
	for( int i = 10; i > 0; --i ) {
		if( ( 11 - i ) * 10 > pct )
			utl << "  ";
		else
			utl << "##";
	}
	utl << "] " << pct << "%";
	
	return utl.str();
}

void sighandler ( int sig ) {
	std::cout << get_time_stamp() << "Caught SIG" << sig << std::endl;
	run = false;
}

int main( int argc, char ** argv ) {
	
	char mask = (char) 1;
	char buffer = (char) 0;
	int counter = 0;
	if( 1 != sizeof( mask ) ) {
		std::cout << "Uh oh! Your char size on this arch isn't a single byte. I can't fit that into my world view." << std::endl;
		return -1;
	}
	
	signal( SIGABRT, &sighandler );
	signal( SIGTERM, &sighandler );
	signal( SIGINT, &sighandler );

	CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
	if( ! capture ) {
		std::cout << get_time_stamp() << "ERROR: Failed to start capture device." << std::endl;
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
	std::stringstream string_util;

	std::string sample_name;
	if( argc > 1 )
		sample_name = argv[1];
	else
		sample_name = get_sample_name();
	
	string_util << sample_name << ".bin";
	std::ofstream file( string_util.str().c_str(), std::ios::out | std::ios::binary | std::ios::app );
	string_util.str("");
	
	int sample_index = file.tellp();
	bool sample_image_captured = ( 0 != sample_index );
	
	if( sample_image_captured )
		std::cout << get_time_stamp() << "Restarting Existing Sample: " << sample_name << " From Byte: " << sample_index << std::endl;
	else
		std::cout << get_time_stamp() << "Starting New Sample: " << sample_name << std::endl;

	int drop_count = 0;
	while( run ) {

		frame = cvQueryFrame( capture );

		if( ! frame ) {
			std::cout << get_time_stamp() << "ERROR: Dropped a frame!" << std::endl;
			++drop_count;
			
			if( MAX_CONSECUTIVE_DROPS <= drop_count ) {
				std::cout << get_time_stamp() << "FATAL: Too many dropped frames." << std::endl;
				break;
			}
			
			continue;
		}
		
		drop_count = 0;

		if( ! sample_image_captured ) {
			string_util << sample_name << ".png";
			if( cvSaveImage( string_util.str().c_str(), frame ) ) {
				std::cout << get_time_stamp() << "Saved image for the sample:" << string_util.str().c_str() << std::endl;
				sample_image_captured = true;
				string_util.str("");
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
						
						// Reset our buffer
						buffer = (char) 0;
						counter = 0;
						
						if( 0 == sample_index % SAMPLE_PROGRESS_INTERVAL && sample_index != 0 ) {
							std::cout << get_time_stamp() << "Progress " << get_progress_bar( sample_index ) << std::endl;
						}
						
						// Full sample?
						if( ++sample_index >= SAMPLE_SIZE ) {
							std::cout << get_time_stamp() << "Finished Sample: " << sample_name << std::endl;
							file.close();
							
							sample_name = get_sample_name();
							
							string_util << sample_name << ".bin";
							file.open( string_util.str().c_str(), std::ios::out | std::ios::binary | std::ios::app );
							string_util.str("");
							
							std::cout  << get_time_stamp() << "Starting New Sample: " << sample_name << std::endl;
							
							row = frame->height;
							sample_index = 0;
							sample_image_captured = false;
							break;
						}

					}
					// Skip a few rows to avoid double registering large scintillations
					row += ROW_SKIP;
					break;
				}
			}
		}

	}

	std::cout << get_time_stamp() << "Shutting Down" << std::endl;

	file.close();
	
	cvReleaseCapture( &capture );

	return 0;
}
