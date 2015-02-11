#ifndef FLX_RETURNBUTTON_H
#define FLX_RETURNBUTTON_H

#include <FL/Fl_Return_Button.H>

#include <flx/flx_signalparms.h>

#include <my/Signal.hpp>

namespace flx {

class Flx_ReturnButton : public Fl_Return_Button {
public:
	my::Signal< Flx_ReturnButton, ActionParm > signalSelected;
public:
    Flx_ReturnButton( int x, int y, int w, int h, const char *pLabel=0 );
    virtual ~Flx_ReturnButton();
    int handle( int );
protected:
	static void staticCallback( Fl_Widget *, void * );
private:
};

} //flx

#endif // FLX_RETURNBUTTON_H
