# -*- coding: utf-8 -*-                               

from opencv import highgui
from opencv import cv

from optparse import OptionParser

# http://www.youtube.com/watch?v=Lvm9gL4KVQg

if __name__ == "__main__" :

	parser = OptionParser()
	parser.add_option( "-g", "--gui", action="store_true", dest="showgui", default=False, help="show the gui; slows things down considerably")
	(options, args) = parser.parse_args()

	if options.showgui:
		highgui.cvNamedWindow( 'Camera', highgui.CV_WINDOW_AUTOSIZE )
	
	camera = highgui.cvCreateCameraCapture( 0 )
	
	if not camera:
		print "Error opening capture device"
		exit( 1 )
	
	highgui.cvSetCaptureProperty( camera, highgui.CV_CAP_PROP_FRAME_WIDTH, 640 )
	highgui.cvSetCaptureProperty( camera, highgui.CV_CAP_PROP_FRAME_HEIGHT, 480 )
	
	while True:
		try:
			i = ( i + 1 ) % 2
			frame = highgui.cvQueryFrame( camera )
			if frame is None:
				print "NO FRAMEZ!?!"
				break

			if options.showgui:
				highgui.cvShowImage( 'Camera', frame )

			k = highgui.cvWaitKey( 10 )

			if k == 'q':
				print 'ESC pressed. Exiting ...'
				break
		except KeyboardInterrupt, e:
			break