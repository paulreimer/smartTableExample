#include "testApp.h"

//--------------------------------------------------------------
testApp::testApp()
{}

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetBackgroundAuto(true);
	ofBackground(255, 255, 255);

	glutSetWindowTitle("SMART Touch Viewer");
}

//--------------------------------------------------------------
void testApp::update()
{}

//--------------------------------------------------------------
void testApp::draw()
{
	handler.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
	handler.mouseMoved(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	handler.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	handler.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
	handler.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}
