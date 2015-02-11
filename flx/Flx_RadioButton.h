#ifndef FLX_RADIOBUTTON_H
#define FLX_RADIOBUTTON_H

#include <FL/Fl_Round_Button.H>

#include <flx/flx_signalparms.h>

#include <my/Signal.hpp>

namespace flx {

class Flx_RadioButton :	public Fl_Round_Button {
public:
	my::Signal< Flx_RadioButton, ActionParm > signalSelected;
public:
	Flx_RadioButton( int x, int y, int w, int h, const char *pLabel = 0 );
	virtual ~Flx_RadioButton(void);
	static void staticCallback( Fl_Widget *, void *pUserdata );
};

} //flx

#endif

