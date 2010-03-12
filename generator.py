# -*- coding: utf-8 -*-                               

#  Copyright (c) 2010 John Hobbs
#  
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#  
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#  
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.

try:
	from ctypes_opencv import *
except:
	from opencv import highgui
	from opencv import cv

from optparse import OptionParser

# http://www.youtube.com/watch?v=Lvm9gL4KVQg

if __name__ == "__main__" :

	parser = OptionParser()
	parser.add_option( "-g", "--gui", action="store_true", dest="showgui", default=False, help="show the gui; slows things down considerably")
	parser.add_option( "-t", "--threshold", type="int", dest="threshold", default=200, help="threshold over which pixel values must be" )
	(options, args) = parser.parse_args()

	if options.showgui:
		highgui.cvNamedWindow( 'Camera', highgui.CV_WINDOW_AUTOSIZE )
	
	camera = highgui.cvCreateCameraCapture( 0 )
	
	if not camera:
		print "Error opening capture device"
		exit( 1 )
	
	highgui.cvSetCaptureProperty( camera, highgui.CV_CAP_PROP_FRAME_WIDTH, 640 )
	highgui.cvSetCaptureProperty( camera, highgui.CV_CAP_PROP_FRAME_HEIGHT, 480 )

	i = 0
	while True:
		try:
			i = ( i + 1 ) % 2
			frame = highgui.cvQueryFrame( camera )
			if frame is None:
				print "NO FRAMEZ!?!"
				break

			x = 0
			for row in frame:
				for col in row:
					x = x + 1
					if col[0] >= options.threshold and col[1] >= options.threshold and col[2] >= options.threshold:
						print x

			if options.showgui:
				highgui.cvShowImage( 'Camera', frame )

			k = highgui.cvWaitKey( 10 )

			if k == 'q':
				print 'ESC pressed. Exiting ...'
				break
		except KeyboardInterrupt, e:
			break
