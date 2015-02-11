#ifndef _FLX_CHECKBUTTON_
#define _FLX_CHECKBUTTON_

#include <FL/Fl_Check_Button.H>

#include <flx/flx_signalparms.h>

#include <my/Signal.hpp>

class Fl_Widget;

namespace flx {

class Flx_CheckButton :	public Fl_Check_Button {
public:
	my::Signal < Flx_CheckButton, ActionParm > signalCheckChanged;
public:
	Flx_CheckButton( int x, int y, int w, int h, const char *pLabel=0 );
	virtual ~Flx_CheckButton(void);
	static void callback( Fl_Widget *, void * );
};

} //flx

#endif

