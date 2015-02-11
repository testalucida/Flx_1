#pragma once

#include <FL/Fl_Input.H>

#include <my/Signal.hpp>
#include <flx/flx_signalparms.h>

namespace flx {

class Flx_Input : public Fl_Input {
public:
	my::Signal < Flx_Input, ActionParm > signalEnterPressed;
public:
	Flx_Input( int x, int y, int w, int h, const char *pLabel=0 );
	virtual ~Flx_Input(void);
	int handle( int );
	static void staticCallback( Fl_Widget *, void * );
};

} //flx

