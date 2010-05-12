#include "TuioThread.h"

//--------------------------------------------------------------
TuioThread::TuioThread()
{}

//--------------------------------------------------------------
void
TuioThread::setup()
{
	startThread(false, false); // non-blocking, non-verbose
}

//--------------------------------------------------------------
void
TuioThread::threadedFunction()
{
	while(1)
	{
		Sleep(1000);
		cout << "Your thread is running" << endl;
		Sleep(1000);
		cout << "Better go catch it" << endl;
	}
}
