#include "testApp.h"

//--------------------------------------------------------------
void
testApp::setup()
{
	discs	= 10;
	sectors	= 9;
	seqfreq	= 100;
	ticks	= 0;
	
	mouseDisc = 0;
	mouseSector = 0;
	
	glutSetWindowTitle("SMART Polar Sequencer");
	ofBackground(255, 255, 255);
	ofSetBackgroundAuto(true);
	ofSetCircleResolution(42);
	ofSetFrameRate(seqfreq);

	windowResized(ofGetWidth(), ofGetHeight());

	map<int,string> instruments;
	instruments[0] = "bass";
	instruments[1] = "clarinet";
	instruments[2] = "english_horn";
	instruments[3] = "flute";
	instruments[4] = "french_horn";
	instruments[5] = "harpsichord";
	instruments[6] = "marimba";
	instruments[7] = "piano";
	instruments[8] = "tuba";

	string soundFile;
	for (int i=0; i<discs; i++)
		for (int j=0; j<sectors; j++)
		{
			sequencer[i][j].r			= make_pair(radius*(float)(i-1)/discs,
													radius*(float)i/discs);
			sequencer[i][j].theta		= make_pair(TWO_PI*j/sectors,
													TWO_PI*(j+1)/sectors);
			
			sequencer[i][j].soundFile	= "wav/" + instruments[j] + "/" + ofToString(i+1) + ".wav";
			sequencer[i][j].enabled		= false;
			sequencer[i][j].setup();
			sequencer[i][j].sound.setVolume(0.6);
		}
}

//--------------------------------------------------------------
void
testApp::update()
{
	static int sweepDiscPrev;

	if (ticks++ > seqfreq)
	{
		ticks = 0;
		sweep = 0.0;
	}
	else
		sweep = ofLerp(sweep, sweep + (radius/seqfreq), 0.95);

	int sweepDisc = discs * sweep/radius;
	
	if (sweepDisc != sweepDiscPrev)
	{
		sweepDiscPrev = sweepDisc;
		for (int i=0; i<sectors; i++)
			if (sequencer[sweepDisc][i].shouldPlay) 
				sequencer[sweepDisc][i].sound.play();
	}
}

//--------------------------------------------------------------
void
testApp::draw()
{
	glPushMatrix();
	glTranslatef(center.x, center.y, 0.);
	
	for (int i=0; i<discs; i++)
	{
		ofNoFill();
		ofSetColor(0x777777);
		ofCircle(0., 0., radius*(float)i/discs);
	}

	float thetaMid;
	for (int i=0; i<discs; i++)
		for (int j=0; j<sectors; j++)
		{
			thetaMid = (sequencer[i][j].theta.first + sequencer[i][j].theta.second)/2;
			if (sequencer[i][j].shouldPlay)
			{
				color.setHSV(thetaMid*RAD_TO_DEG, 0.8, 1., 0.9);
				color.setGL();
				sequencer[i][j].draw();
			}
		}

	if (mouseDisc < discs+1 && mouseDisc > 0
		&& mouseSector < sectors)
	{
		thetaMid = (sequencer[mouseDisc][mouseSector].theta.first
					+ sequencer[mouseDisc][mouseSector].theta.second)/2;
		color.setHSV(thetaMid*RAD_TO_DEG, 1., 1., 1.);	
		color.setGL();
		sequencer[mouseDisc][mouseSector].draw();
	}

	ofNoFill();
	ofSetLineWidth(5.0);
	ofSetColor(0x1111aa);
	ofCircle(0., 0., sweep);
	ofSetLineWidth(1.0);

	glPopMatrix();

	handler.draw();
}

//--------------------------------------------------------------
void
testApp::keyPressed(int key)
{}

//--------------------------------------------------------------
void
testApp::keyReleased(int key)
{}

//--------------------------------------------------------------
void
testApp::mouseMoved(int x, int y)
{
	ofPoint polar	= polarFromCartesian(ofPoint(x,y), center);
	
	mouseDisc		= ((float)polar.x / radius) * discs + 1;
	mouseSector		= ((float)polar.y / TWO_PI) * sectors;
	
	handler.mouseMoved(x, y);
}

//--------------------------------------------------------------
void
testApp::mouseDragged(int x, int y, int button)
{
	handler.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void
testApp::mousePressed(int x, int y, int button)
{
	ofPoint polar	= polarFromCartesian(ofPoint(x,y), center);
	
	int disc	= ((float)polar.x / radius) * discs + 1;
	int sector	= ((float)polar.y / TWO_PI) * sectors;
	
	sequencer[disc][sector].togglePlaying();

	handler.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void
testApp::mouseReleased(int x, int y, int button)
{
	handler.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void
testApp::windowResized(int w, int h)
{
	center.set(w/2, h/2);
	radius = MIN(center.x, center.y);
}
