#pragma once

#include "ofMain.h"
#include "Segment.h"
#include "msaColor.h"
#include "SMART Touch Handler.h"
#include "TuioThread.h"

class testApp
: public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	//	void touched();
	//	void untouched();
	//	void dragged();
	
	//	ofEvent			touched;
	//	ofEvent			untouched;
	//	ofEvent			dragged;
	
	float radius;
	ofPoint center;

	int discs, sectors;

	int mouseDisc, mouseSector;
	
	map<int, map<int, Segment> > sequencer;
	
	msaColor color;
	
	int seqfreq;
	float sweep;
	int ticks;

	SMARTTouchHandler handler;

	TuioThread tuioThread;
};
