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

#include "ofxCvOpticalFlowLK.h"

ofxCvOpticalFlowLK::ofxCvOpticalFlowLK(){
	captureColsStep = 4;
	captureRowsStep = 4;
	isSetup = false;
	minVector = 0;
	maxVector = 1000;
}

ofxCvOpticalFlowLK::~ofxCvOpticalFlowLK(void){
	if(isSetup){
		cvReleaseImage(&vel_x);
		cvReleaseImage(&vel_y);
	}
}
	
void ofxCvOpticalFlowLK::allocate(int _w, int _h, int captureStep){
	captureWidth = _w;
	captureHeight = _h;

	vel_x = cvCreateImage( cvSize( captureWidth ,captureHeight ), IPL_DEPTH_32F, 1  );
	vel_y = cvCreateImage( cvSize( captureWidth ,captureHeight ), IPL_DEPTH_32F, 1  );
	
    cvSetZero(vel_x);
    cvSetZero(vel_y);
	
	isSetup = true;
}

void ofxCvOpticalFlowLK::setCalcStep(int _cols, int _rows){
	captureColsStep = _cols;
	captureRowsStep = _rows;
}

void ofxCvOpticalFlowLK::calc( ofxCvGrayscaleImage & pastImage, ofxCvGrayscaleImage & currentImage, int size){
	cvCalcOpticalFlowLK( pastImage.getCvImage(), currentImage.getCvImage(), cvSize( size, size), vel_x, vel_y );

}

void ofxCvOpticalFlowLK::draw(){
	
	draw(captureWidth, captureHeight);	
}

void ofxCvOpticalFlowLK::draw(int width, int height){

	ofPushStyle();
	
//	float scalex = (float) width / captureWidth;
//	float scaley = (float) height / captureHeight;
	
	int x, y, dx, dy;
	for ( y = 0; y < captureHeight; y+=captureRowsStep ){
		for ( x = 0; x < captureWidth; x+=captureColsStep ){
			
//			dx = (int)cvGetReal2D( vel_x, y, x );
//			dy = (int)cvGetReal2D( vel_y, y, x );
//			
			ofVec2f vec = flowAtPoint(x, y);

			ofLine(x,y,(x+vec.x),(y+vec.y));
			
		}
	}
	
	ofPopStyle();
	
}

ofVec2f ofxCvOpticalFlowLK::flowAtPoint(int x, int y){
	if(x >= captureWidth || x < 0 || y >= captureHeight || y < 0){

		return ofVec2f(0.0, 0.0);
	}
	float fdx = cvGetReal2D( vel_x, y, x );
	float fdy = cvGetReal2D( vel_y, y, x );
	float mag2 = fdx*fdx + fdy*fdy;
	if(  mag2 > maxVector*maxVector){
		//return a normalized vector of the magnitude size
		return ( ofVec2f(fdx,fdy) / sqrt(mag2) ) * maxVector;
	}
	if( mag2 < minVector*minVector){
		//threhsold to 0
		return ofVec2f(0,0);
	}
	return ofVec2f(fdx, fdy);
}

ofVec2f ofxCvOpticalFlowLK::flowInRegion(ofRectangle roi)
{
	return flowInRegion(roi.x, roi.y, roi.width, roi.height);
}

ofVec2f ofxCvOpticalFlowLK::flowInRegion(float x, float y, float w, float h){
	ofVec2f topLeft, bottomRight, total(0,0);
	boundsForRect(x, y, w, h, &topLeft, &bottomRight);
	for (int j = topLeft.y; j < bottomRight.y; j++) {
		for(int i = topLeft.x; i < bottomRight.x; i++){
			total += flowAtPoint(i, j);
		}
	}
	return total; 
}

void ofxCvOpticalFlowLK::boundsForRect(float x, float y, float w, float h, ofVec2f* topLeft, ofVec2f* bottomRight)
{
	*topLeft     = ofVec2f(fmax(0, x), fmax(0, y));
	*bottomRight = ofVec2f( (x + w > captureWidth) ? captureWidth :  x + w, 
						   (y + h > captureHeight) ? captureHeight :  y + h);
	
}

ofVec2f ofxCvOpticalFlowLK::averageFlowInRegion(float x, float y, float w, float h)
{
	ofVec2f topLeft, bottomRight, total(0,0);
	boundsForRect(x, y, w, h, &topLeft, &bottomRight);
	for (int j = topLeft.y; j < bottomRight.y; j += captureRowsStep) {
		for(int i = topLeft.x; i < bottomRight.x; i += captureColsStep) {
			total += flowAtPoint(i, j);
		}
	}

	return total / ( (bottomRight.x - topLeft.x) * (bottomRight.y - topLeft.y));
}

ofVec2f ofxCvOpticalFlowLK::averageFlowInRegion(ofRectangle roi)
{
	return averageFlowInRegion(roi.x, roi.y, roi.width, roi.height);
}

