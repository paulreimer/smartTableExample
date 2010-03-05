#include "SMART Touch Contact.h"

#include "ofTrueTypeFont.h"

ofTrueTypeFont font;

SMARTContact::SMARTContact()
{
	if (!font.bLoadedOk)
		font.loadFont("FontinSans.ttf", 8);

	m_device = UnknownDevice;
}

SMARTMultiTouchContact::SMARTMultiTouchContact()
{
	m_device = MultiTouchDevice;
	
	// Position
	m_posx = 0.;
	m_posy = 0.;

	// Velocity
	m_velx = 0.;
	m_vely = 0.;

	// Width & height		
	m_width = 0.;
	m_height = 0.;

	// PCA
	m_angle = 0.;
	m_major = 0.;
	m_minor = 0.;

	// Pressure
	m_intensity = 0.;
}

void SMARTMultiTouchContact::draw()
{
	// Draw axis aligned bounding box and intensity
	ofSetColor(0x0000ff);
	ofRect(m_posx - (m_width/2.), m_posy - (m_height/2.), m_width, m_height);
	ofRect(m_posx - (m_width/2.), m_posy - (m_height/2.), m_width, m_height);

	// Draw PCA ellipse
	ofPushMatrix();
	// Translate to the object center
	ofTranslate(-m_minor*0.5f, -m_major*0.5f, 0.);
	// Rotate around the object center
	ofRotate(-m_angle, 0., 0., 1.);
	// Translate to the desired location
	ofTranslate(m_posx, m_posy, 0.);
	// Draw the ellipse
	ofSetColor(0xff0000);
	ofEllipse(0., 0., m_minor, m_major);
	// Draw the arrow
	ofSetColor(0xffffff);
	ofLine(m_minor*0.5f, m_major, m_minor*0.5f, 0.0f);		// Major axis
	ofLine(m_minor*0.5f, 0., m_minor*0.25f, m_major*0.25f);	// Arrow head
	ofLine(m_minor*0.5f, 0., m_minor*0.75f, m_major*0.25f);	
	ofLine(m_minor, m_major*0.5f, 0.0f, m_major*0.5f);		// Minor axis
	// Reset the transform
	ofPopMatrix();
	// Draw velocity vector
	ofSetColor(0x00ff00);
	ofSetLineWidth(2.);
	ofLine(m_posx, m_posy, m_posx + m_velx, m_posy + m_vely);

	// Draw center
	ofSetLineWidth(1.);
	ofSetColor(0x000000);
	ofLine(m_posx, m_posy - CENTER, m_posx, m_posy + CENTER);
	ofLine(m_posx - CENTER, m_posy, m_posx + CENTER, m_posy);

	// Draw contact ID
	font.drawString("Multi: "+ofToString(m_id), m_posx, m_posy - TEXT);
}

void SMARTMultiTouchContact::UpdateProperties(int id, 
	float x_position, float y_position, 
	float x_velocity, float y_velocity, 
	float width, float height, float intensity, 
	float angle, float major, float minor)
{
	m_id = id;
	m_posx = x_position;
	m_posy = y_position;
	m_velx = x_velocity;
	m_vely = y_velocity;

	m_width = width;
	m_height = height;
	m_intensity = intensity;
	m_major = major;
	m_minor = minor;
	m_angle = angle;	
}

SMARTTouchContact::SMARTTouchContact()
{
	if (!font.bLoadedOk)
		font.loadFont("FontinSans.ttf", 8);

	m_device = TouchDevice;
	m_x = 0;
	m_y = 0;
	m_z = 0;
}

void SMARTTouchContact::draw()
{
	if(m_tool == SB_PEN)
	{
		ofLine(m_x - SIZE, m_y - SIZE, m_x + SIZE, m_y + SIZE);
		ofLine(m_x - SIZE, m_y + SIZE, m_x + SIZE, m_y - SIZE);

	}
	else if(m_tool == SB_CIRCLE)
	{
		ofEllipse(m_x - SIZE, m_y - SIZE, 2 * SIZE, 2 * SIZE);

	}
	else if(m_tool == SB_RECTANGLE)
	{
		ofRect(m_x - SIZE, m_y - SIZE, 2 * SIZE, 2 * SIZE);
	}
	else {
		ofSetColor(0x000000);
		ofLine(m_x - SIZE, m_y - SIZE, m_x + SIZE, m_y + SIZE);
		ofLine(m_x - SIZE, m_y + SIZE, m_x + SIZE, m_y - SIZE);
	}

	font.drawString("Touch: "+ofToString(m_id), m_x, m_y - (4*SIZE));
}

void SMARTTouchContact::UpdatePosition(int x, int y, int z, int id)
{
	m_x = x;
	m_y = y;
	m_z = z;

	m_id = id;
}

void SMARTTouchContact::UpdateTool(SBSDK_TOOL_TYPE tool, COLORREF color, int width)
{
	m_tool = tool;
	m_width = width;
}

SMARTMouseContact::SMARTMouseContact()
{
	if (!font.bLoadedOk)
		font.loadFont("FontinSans.ttf", 8);

	m_device = MouseDevice;
	m_x = 0;
	m_y = 0;
	m_left = false;
	m_middle = false;
	m_right = false;
}

void SMARTMouseContact::draw()
{
	ofSetColor(0x000000);
	ofLine(m_x, m_y - SIZE, m_x, m_y + SIZE);
	ofLine(m_x - SIZE, m_y, m_x + SIZE, m_y);

	ofSetLineWidth(BUTTON);
	if(m_left)
	{
		ofSetColor(0xff0000);
		ofLine(m_x - SIZE, m_y - OFFSET, m_x - SIZE, m_y - SIZE);
	}

	if(m_middle)
	{
		ofSetColor(0x00ff00);
		ofLine(m_x, m_y - OFFSET, m_x, m_y - SIZE);
	}

	if(m_right)
	{
		ofSetColor(0x0000ff);
		ofLine(m_x + SIZE, m_y - OFFSET, m_x + SIZE, m_y - SIZE);
	}
	ofSetLineWidth(1.);
	// Draw contact ID
	font.drawString("Mouse: ", m_x+8, m_y - (4*SIZE));
}

void SMARTMouseContact::UpdatePosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void SMARTMouseContact::UpdateButtons(bool left_button, bool mid_button, bool right_button)
{
	m_left = left_button;
	m_middle = mid_button;
	m_right = right_button;
}

void SMARTMouseContact::SetButtons(bool left_button, bool mid_button, bool right_button)
{
	UpdateButtons(left_button || m_left, mid_button || m_middle, right_button || m_right);
}


void SMARTMouseContact::UnsetButtons(bool left_button, bool mid_button, bool right_button)
{
	UpdateButtons((!left_button) && m_left, (!mid_button) && m_middle, (!right_button) && m_right);
}

bool SMARTMouseContact::IsAnyButton()
{
	return m_left | m_middle | m_right;
}
