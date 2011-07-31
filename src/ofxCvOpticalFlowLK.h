//
// ofxCvOpticalFlowLK.h - a OpenCV cvOpticalFlowLK warpper for openFrameworks
//
// Copyright (C) 2008 Takashi Maekawa <takachin@generative.info> 
//     All rights reserved.
//     This is free software with ABSOLUTELY NO WARRANTY.
//
// You can redistribute it and/or modify it under the terms of 
// the GNU Lesser General Public License.
//

#pragma once

#include "ofMain.h"
#include "ofxCvConstants.h"
#include "ofxCvGrayscaleImage.h"

class ofxCvOpticalFlowLK 
{
  public:
	ofxCvOpticalFlowLK(void);
	~ofxCvOpticalFlowLK(void);
		
	void allocate(int _w, int _h, int captureStep = 4);

	void calc( ofxCvGrayscaleImage & pastImage,
			   ofxCvGrayscaleImage & currentImage,
			  int size);

	void setCalcStep(int _cols, int _rows);
		
	void reset();
	void draw();
	void draw(int width, int height);
	
	ofVec2f flowAtPoint(int x, int y);
	ofVec2f flowInRegion(float x, float y, float w, float h);
	ofVec2f flowInRegion(ofRectangle roi);
	
	ofVec2f averageFlowInRegion(float x, float y, float w, float h);
	ofVec2f averageFlowInRegion(ofRectangle roi);
	
	//Used to filter noisey or erroneous vectors
	float minVector;
	float maxVector;

private:

	void boundsForRect(float x, float y, float w, float h, ofVec2f* topLeft, ofVec2f* bottomRight);
	
	int captureWidth;
	int captureHeight;

	bool isSetup;

	int captureColsStep;
	int captureRowsStep;

	IplImage* vel_x;
	IplImage* vel_y;
	

};
