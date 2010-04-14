#include "SMART Touch Handler.h"

using namespace std;

SMARTTouchHandler::SMARTTouchHandler()
{
	// Remember window handle for later
//	m_hwnd = FindWindow(TEXT("GLUT"), TEXT("WINDOWNAME"));
	//get the HWND
	m_hwnd = WindowFromDC(wglGetCurrentDC());
	m_mouse = NULL;
	
	cout << "Attaching our window (hwnd: " << (INT64) m_hwnd << ") with the SMART Board SDK" << endl;
	// Attach the SMART Board SDK to our window handle
	m_sdk.SBSDKAttach(CSBSDKWnd(m_hwnd));
	//m_sdk.SBSDKAttachWithMsgWnd(CSBSDKWnd(m_hwnd), false, CSBSDKWnd(m_hwnd));
	if(!m_sdkadv.SBSDKIsConnected())
		cout << "Error: Failed to connect to the SBSDK" << endl;

	cout << "Registering touch event handlers" << endl;
	// Enable us to handle SDK events
	m_sdk.SetEventHandler(this);
	// Enable us to handle advanced SDK events
	m_sdkadv.SetEventHandler(this);

	// Enable us to handle multi-touch SDK events
	cout << "Registering multi-touch event handler" << endl;
	SBSDK_ERR err = m_sdkadv.SBSDKSendMultiPointContacts(true);
	if(err != SBE_OK)
		cout << "Error: " << "Unable to connect to a multi-touch device [" << err << "]" << endl;

	// Tell all boards to never send mouse events
	// Note: Since the SMART Table has no pen tray, this command is 
	// required before the SDK will send us multi-touch data
	m_sdk.SBSDKSendMouseEvents(CSBSDKWnd(m_hwnd), SB_MEF_NEVER, -1);

	// Query for SMART Boards
	cout << "Discovered " << m_sdk.SBSDKGetConnectedBoardCount() << " SMART Board devices" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
}


SMARTTouchHandler::~SMARTTouchHandler()
{
	// Detach event handlers
	m_sdk.SetEventHandler(NULL);
	m_sdkadv.SetEventHandler(NULL);
}

void SMARTTouchHandler::MessageReceived()
{
	m_sdk.SBSDKProcessData();
}

void SMARTTouchHandler::draw()
{
	// Draw our contacts
	SMARTContactListIterator itr;
	for(itr=m_contacts.begin(); itr!=m_contacts.end(); itr++)
		(*itr)->draw();
}

void SMARTTouchHandler::OnMultiPointContact(CSBSDKContactList &list)
{
	SMARTMultiTouchContact*	contact = NULL;
	CSBSDKContact		data;
	CSBSDKWnd			wnd;
	SBSDK_CONTACT_STATE state;

	int   id;				// Unique contact id
	DWORD timestamp;		// Timestamp - GetTickCount()
	float posx, posy;		// Position
	float velx, vely;		// Velocity
	float width, height;	// Width & height
	float major, minor;		// PCA major & minor axis
	float angle;			// PCA angle
	float intensity;		// Pressure

	for (int i = 0; i < list.GetCount(); i++)
	{
		list.GetAt(i, data);
		data.GetContactData(wnd, state, timestamp, id, posx, posy, velx, vely, 
			width, height, angle, intensity, major, minor);

		switch(state)
		{
			case SB_CONTACT_DOWN:
				cout << "Multi[" << id << "] down" << endl;

				contact = new SMARTMultiTouchContact();
				contact->UpdateProperties(id, posx, posy, velx, vely, 
					width, height, intensity, angle, major, minor);

				// Add to the main list
				m_contacts.push_front((SMARTContact*) contact);
				// Add to the touch map
				m_multi[id] = (SMARTContact*) contact;
				break;

			case SB_CONTACT_MOVE:
				//cout << "Multi[" << id << "] move" << endl;

				contact = (SMARTMultiTouchContact*) m_multi[id];
				if(contact)
				{
					contact->UpdateProperties(id, posx, posy, velx, vely, 
						width, height, intensity, angle, major, minor);
				}
				break;

			case SB_CONTACT_UP:
				cout << "Multi[" << id << "] up" << endl;

				contact = (SMARTMultiTouchContact*) m_multi[id];
				if(contact)
				{
					// Remove from the main list
					m_contacts.remove((SMARTContact*) contact);
					// Remove from the touch map
					delete m_multi[id];
					m_multi[id] = NULL;
				}
				break;

			default:
				cout << "Multi[" << id << "] other" << endl;
				break;
		}
	}
}

void SMARTTouchHandler::OnXYDown(int x, int y, int z, int iPointerID)
{
	SMARTTouchContact* contact = NULL;

	cout << "Touch[" << iPointerID << "] down" << endl;

	contact = new SMARTTouchContact();
	if(contact)
	{
		SBSDK_TOOL_TYPE tool = m_sdk.SBSDKGetToolType(iPointerID);
		COLORREF color = m_sdk.SBSDKGetToolColor(iPointerID);
		int width = m_sdk.SBSDKGetToolWidth(iPointerID);

		contact->UpdateTool(tool, color, width);
		contact->UpdatePosition(x, y, z, iPointerID);

		// Add to the main list
		m_contacts.push_front((SMARTContact*) contact);
		// Add to the touch map
		m_touch[iPointerID] = (SMARTContact*) contact;
	}	
}

void SMARTTouchHandler::OnXYUp(int x, int y, int z, int iPointerID)
{
	SMARTTouchContact* contact = NULL;

	cout << "Touch[" << iPointerID << "] up" << endl;

	contact = (SMARTTouchContact*) m_touch[iPointerID];
	if(contact)
	{
		// Remove from the main list
		m_contacts.remove((SMARTContact*) contact);
		// Remove from the touch map
		delete m_touch[iPointerID];
		m_touch[iPointerID] = NULL;
	}
}

void SMARTTouchHandler::OnXYMove(int x, int y, int z, int iPointerID)
{
	SMARTTouchContact* contact = NULL;

	contact = (SMARTTouchContact*) m_touch[iPointerID];
	if(contact)
	{		
		contact->UpdatePosition(x, y, z, iPointerID);
	}
}

//--------------------------------------------------------------
void SMARTTouchHandler::mouseMoved(int x, int y)
{
	if(m_mouse!=NULL)
		m_mouse->UpdatePosition(x, y);
}

//--------------------------------------------------------------
void SMARTTouchHandler::mousePressed(int x, int y, int button)
{
	if(m_mouse==NULL)
	{
		cout << "Mouse down" << endl;
		m_mouse = new SMARTMouseContact();
		m_contacts.push_front((SMARTContact*) m_mouse);
		
		if(GetCapture() != m_hwnd)
		{
			SetCapture(m_hwnd);
		}
	}

	if(m_mouse!=NULL)
	{
		m_mouse->UpdatePosition(x, y);
		m_mouse->SetButtons(button==GLUT_LEFT_BUTTON, button==GLUT_MIDDLE_BUTTON, button==GLUT_RIGHT_BUTTON);
	}
}

//--------------------------------------------------------------
void SMARTTouchHandler::mouseReleased(int x, int y, int button)
{
	if(m_mouse!=NULL)
	{
		m_mouse->UpdatePosition(x, y);
		m_mouse->UnsetButtons(button==GLUT_LEFT_BUTTON, button==GLUT_MIDDLE_BUTTON, button==GLUT_RIGHT_BUTTON);
		if(!m_mouse->IsAnyButton())
		{
			cout << "Mouse up" << endl;
			
			m_contacts.remove((SMARTContact*) m_mouse);
			delete m_mouse;
			m_mouse = NULL;
			
			ReleaseCapture();
		}
	}
}

//--------------------------------------------------------------
void SMARTTouchHandler::mouseDragged(int x, int y, int button)
{
	if(m_mouse!=NULL)
		m_mouse->UpdatePosition(x, y);
}