#pragma once

#include "ofxThread.h"

class TuioThread
: public ofxThread
{
public:
	TuioThread();

	void setup();
	void threadedFunction();
};
