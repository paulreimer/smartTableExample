#pragma once

#include "ofMain.h"

#include "SBSDK2.h"

enum SMARTDevice
{
	UnknownDevice,
	MouseDevice,
	TouchDevice,
	MultiTouchDevice,
};

class SMARTContact
{
	public:
		SMARTContact();
		virtual ~SMARTContact() {}

		virtual void draw() {}

		virtual SMARTDevice GetDevice() { return m_device; }

	protected:
		SMARTDevice m_device;
};

class SMARTMultiTouchContact : public SMARTContact
{
	public:
		SMARTMultiTouchContact();
		virtual ~SMARTMultiTouchContact() {}

		virtual void draw();

		virtual void UpdateProperties(int id, 
			float x_position, float y_position, 
			float x_velocity, float y_velocity, 
			float width, float height, float intensity, 
			float angle, float major, float minor);

	protected:
		int		m_id;				// Unique identifier
		float	m_posx, m_posy;		// Position
		float	m_velx, m_vely;		// Velocity
		float	m_width, m_height;	// Width & height
		float	m_major, m_minor;	// PCA major & minor axis
		float	m_angle;			// PCA angle
		float	m_intensity;		// Pressure	

		// Rendering constants
		static const int CENTER	= 3;
		static const int TEXT	= 25;
};

class SMARTTouchContact : public SMARTContact
{
	public:
		SMARTTouchContact();
		virtual ~SMARTTouchContact() {}

		virtual void draw();

		virtual void UpdatePosition(int x, int y, int z, int id);
		virtual void UpdateTool(SBSDK_TOOL_TYPE tool, COLORREF color, int width);

	protected:
		int		m_id;			// Device id
		int		m_x, m_y;		// Position
		int		m_z;			// Depth

		SBSDK_TOOL_TYPE	m_tool;	// Tool type
		int		m_width;		// Tool width

		// Rendering constants
		static const int SIZE	= 5;
};

class SMARTMouseContact : public SMARTContact
{
	public:
		SMARTMouseContact();
		virtual ~SMARTMouseContact() {}

		virtual void draw();

		virtual void UpdatePosition(int x, int y);
		virtual void UpdateButtons(bool left_button, bool mid_button, bool right_button);

		virtual void SetButtons(bool left_button, bool mid_button, bool right_button);
		virtual void UnsetButtons(bool left_button, bool mid_button, bool right_button);

		virtual bool IsAnyButton();

	protected:
		int		m_x, m_y;		// Position
		bool	m_left, m_middle, m_right;

		// Rendering constants
		static const int SIZE	= 5;
		static const int BUTTON = 3;
		static const int OFFSET = 3;
};

