#include "OpencvGestureDetector.h"
#include <opencv2\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

extern "C"
{
	using namespace cv;
	
	// Function that read a Haar cascades file (object detectors) from a path
	CascadeClassifier* InitCascade(const char *filePath) {
		
		CascadeClassifier* cascade = new CascadeClassifier();
		cascade->load(filePath);

		return cascade;

	}

	void DestroyCascade(CascadeClassifier* cascade)
	{
		delete cascade;
	}

	
	/** 
		Function that detects a specific gesture
		Returns the number of detected gestures of the specified type
		
		@cascade instance to be used for detection (*)
		@raw: pixels array (image)
		@width: width of the image
		@height: height of the image
		@id: id of the gesture to recognize
		@outGestures: array of detected gestures (positions and sizes)
		@maxOutGestureCount: maximum number of gestures to detect

		(*) In Unity, this functions must be static in order to use it with DLLImport. If the function is static it is difficult to use different
		instances to reference multiple cascades. With this pointer we pass a reference to the cascade that we want to use.
		The other option was to create a list of cascades in here and call them using an id.
		https://answers.unity.com/questions/1200157/nonstatic-extern-functions-from-dll-plugin-import.html
	*/
	int DetectGesture(CascadeClassifier* cascade, Color32 * raw, int width, int height, DetectedGesture* outGestures, int maxOutGestureCount, float scaleFactor, int minSize)
	{
		// Verify if the Haar Cascade is loaded
		if (cascade->empty()) {
			return -1;
		}


		// Create an OpenCV matrix from the raw pixel array
		Mat frame(height, width, CV_8UC4, raw);

		/*
		// How to access a specific pixel
		Vec4b intensity = frame.at<Vec4b>(height - 1, width - 1);
		unsigned char red = intensity.val[0];
		unsigned char green = intensity.val[1];
		unsigned char blue = intensity.val[2];
		unsigned char alpha = intensity.val[3];
		*/

		// List of detected gestures
		std::vector<Rect> foundGestures;
		
		// Generating gray scale of the image and equalizing the histogram (improve contrast)
		Mat frame_gray;
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		cv::equalizeHist(frame_gray, frame_gray);

		// Use the classifier
		cascade->detectMultiScale(frame_gray, foundGestures, scaleFactor, 2, 0 | CASCADE_SCALE_IMAGE, Size(minSize, minSize));

		// Save the detected gestures into the output array
		int detectedCount = 0;
		for (size_t i = 0; i < foundGestures.size(); i++)
		{
			// Save the gesture
			outGestures[i] = DetectedGesture(foundGestures[i].x, foundGestures[i].y, foundGestures[i].width, foundGestures[i].height);
			detectedCount++;

			// Check if the  maximum number of detected gesture is not reached
			if (detectedCount == maxOutGestureCount)
				break;

		}

		return detectedCount;

	}
	
}