#ifndef _FLX_CHOICE_
#define _FLX_CHOICE_

#include <FL/Fl_Choice.H>

#include <flx/flx_signalparms.h>

#include <my/Signal.hpp>

class Fl_Widget;

namespace flx {

class Flx_Choice : public Fl_Choice {
public:
	my::Signal < Flx_Choice, ActionParm > signalSelected;
	static void static_callback( Fl_Widget *, void * );
public:
	Flx_Choice( int x, int y, int w, int h, const char *pLabel=0 );
	~Flx_Choice(void);
};

} //flx

#endif

