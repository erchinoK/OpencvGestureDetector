#pragma once
#include <opencv2\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


// Structure to interact with Unity (pixels of the image)
struct Color32
{
	unsigned char a;
	unsigned char b;
	unsigned char g;
	unsigned char r;

};

// Structure to store a detected gesture: position and size
// Must be the same in Unity
struct DetectedGesture
{
	int x;
	int y;
	int width;
	int height;
	DetectedGesture(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
};

extern "C"
{
	
		using namespace cv;

		// Function that reads the Haar cascades (object detectors) from the file system (xml files)
		CascadeClassifier* InitCascade(const char *filePath);

		void DestroyCascade(CascadeClassifier* cascade);

		// Function that detects gestures using the classifiers with OpenCV API
		int DetectGesture(CascadeClassifier* cascade, Color32 * raw, int width, int height, DetectedGesture* outGestures, int maxOutGestureCount, float scaleFactor = 1.1, int minSize = 30);

	
}

