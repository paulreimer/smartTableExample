#pragma once

#include <windows.h>

#include "SBSDK2.h"
#include "SBSDK2Advanced.h"

#include "SMART Touch Contact.h"

typedef std::list<SMARTContact*> SMARTContactList;
typedef std::list<SMARTContact*>::iterator SMARTContactListIterator;

typedef std::map<int, SMARTTouchContact*> SMARTTouchContactMap;
typedef std::map<int, SMARTTouchContact*>::iterator SMARTTouchContactMapIterator;

typedef std::map<int, SMARTContact*> SMARTContactMap;
typedef std::map<int, SMARTContact*>::iterator SMARTContactMapIterator;

class SMARTTouchHandler : public CSBSDK2EventHandler,
						  public CSBSDK2AdvancedEventHandler
{
	public:
		SMARTTouchHandler(/* HWND hwnd */);
		virtual ~SMARTTouchHandler();

		void MessageReceived();

		void draw();

		// Mouse event handlers
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

	protected:
		// Single touch data handlers
		void OnXYDown(int x, int y, int z, int iPointerID);
		void OnXYUp(int x, int y, int z, int iPointerID);
		void OnXYMove(int x, int y, int z, int iPointerID);

		// Multi-touch event handler
		void OnMultiPointContact(CSBSDKContactList &list);

		//Tools
		//void OnNoTool(int iPointerID);
		//void OnPen(int iPointerID);
		//void OnXMLToolChangeW(int iBoardID, wchar_t *szXMLTool);

		//Buttons
		//void OnClear(int iPointerID);
		//void OnPenTrayButton(SBSDK_BUTTON_ACTION baButton, int iPointerID);

	private:
		//SMARTContactListIterator FindMouseContact();

	private:
		// SBSDK
		CSBSDK2				m_sdk;		// SBSDK handle
		CSBSDK2Advanced		m_sdkadv;	// SBSDK advanced handle

		// Main contact list
		SMARTContactList	m_contacts;	// List of current contacts

		// Device specific contact maps
		SMARTMouseContact*	m_mouse;	// Mouse contact device (single input)
		SMARTContactMap		m_touch;	// Map of touch contacts
		SMARTContactMap		m_multi;	// Map of multi-touch contacts

		HWND				m_hwnd;		// Window handle
};
