#include "Segment.h"
#include "ofMath.h"

//--------------------------------------------------------------
Segment::Segment()
{
	shouldPlay	= false;
	circRes		= 42;
}

//--------------------------------------------------------------
void
Segment::setup()
{
	sound.loadSound(soundFile);	
	
	inner = make_pair(cartesianFromPolar(ofPoint(r.first,theta.first)),
					  cartesianFromPolar(ofPoint(r.first,theta.second)));

	outer = make_pair(cartesianFromPolar(ofPoint(r.second,theta.first)),
					  cartesianFromPolar(ofPoint(r.second,theta.second)));
}

//--------------------------------------------------------------
void
Segment::update()
{}

//--------------------------------------------------------------
void
Segment::draw()
{
	float angle		= theta.first;
	float angleIncr	= (TWO_PI/circRes)*(theta.second - theta.first);
	int	steps		= (theta.second - theta.first)/angleIncr;
	
	ofFill();
	ofBeginShape();
	ofVertex(inner.first.x, inner.first.y);
	ofVertex(outer.first.x, outer.first.y);
	
	for (int i=0; i<steps; i++)
	{
		angle += angleIncr;
		ofVertex(r.second*cos(angle - PI),
				 r.second*sin(angle - PI));
	}
	
	ofVertex(outer.second.x, outer.second.y);
	ofVertex(inner.second.x, inner.second.y);
	
	angle = theta.second; // (it should)
	for (int i=0; i<steps; i++)
	{
		angle -= angleIncr;
		ofVertex(r.first*cos(angle - PI),
				 r.first*sin(angle - PI));
	}

	ofEndShape(true);
}

//--------------------------------------------------------------
void
Segment::togglePlaying()
{
	shouldPlay = !shouldPlay;
	if (sound.getIsPlaying())
		sound.stop();
}

//--------------------------------------------------------------
ofPoint
polarFromCartesian(ofPoint p, ofPoint origin)
{
	return ofPoint(ofDist(p.x, p.y, origin.x, origin.y),
				   PI + atan2(p.y-origin.y, p.x-origin.x));
}

//--------------------------------------------------------------
ofPoint
cartesianFromPolar(ofPoint p, ofPoint origin)
{
	return ofPoint(origin.x + p.x*cos(p.y - PI),
				   origin.y + p.x*sin(p.y - PI));
}
