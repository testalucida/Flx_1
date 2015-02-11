#pragma once

#include <FL/Fl_Multiline_Input.H>

#include <my/Signal.hpp>

namespace flx {
    
struct ActionParm;

class Flx_MultiLineInput : public Fl_Multiline_Input {
public:
    my::Signal< Flx_MultiLineInput, bool > signalFocus;
    my::Signal< Flx_MultiLineInput, ActionParm > signalChanged;
public:
	Flx_MultiLineInput( int x, int y, int w, int h, const char *pLabel=0 );
	virtual ~Flx_MultiLineInput(void);
protected:
    int handle( int e );
private:
    static void staticCallback( Fl_Widget *, void * );
};

}

