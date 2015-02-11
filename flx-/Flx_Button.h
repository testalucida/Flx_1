#ifndef _FLX_BUTTON_
#define _FLX_BUTTON_

#include <FL/Fl_Button.H>

#include <flx/flx_signalparms.h>

#include <my/Signal.hpp>

namespace flx {

class Flx_Button : public Fl_Button {
public:
	my::Signal< Flx_Button, ActionParm > signalSelected;
public:
	Flx_Button( int x, int y, int w, int h, const char *pLabel=0 );
	virtual ~Flx_Button(void);
	int handle( int );
	void simulateEnter();
protected:
	static void staticCallback( Fl_Widget *, void * );

};

} //flx

#endif

