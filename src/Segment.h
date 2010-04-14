#pragma once

#include "ofSoundPlayer.h"
#include "ofGraphics.h"

class Segment
{
public:
	
	Segment();

	pair<float,float>		r;
	pair<float,float>		theta;
	
	void setup();
	void update();
	void draw();

	void togglePlaying();

	void touched();
	void untouched();
	void dragged();
	
	string			soundFile;
	bool			enabled;
	bool			shouldPlay;

	ofSoundPlayer	sound;

protected:
//	ofEvent			touched;
//	ofEvent			untouched;
//	ofEvent			dragged;

	pair<ofPoint,ofPoint>	inner;
	pair<ofPoint,ofPoint>	outer;
	
	
	float			circRes;

};

ofPoint polarFromCartesian(ofPoint p, ofPoint origin=ofPoint(0,0));
ofPoint cartesianFromPolar(ofPoint p, ofPoint origin=ofPoint(0,0));
