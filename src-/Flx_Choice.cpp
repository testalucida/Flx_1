#include <flx/Flx_Choice.h>

#include <FL/fl_draw.H>

namespace flx {

Flx_Choice::Flx_Choice( int x, int y, int w, int h, const char *pLabel )
	: Fl_Choice( x, y, w, h, pLabel )
{
	box( FL_FLAT_BOX );
	textfont( fl_font() );
	textsize( fl_size() );
	labelfont( fl_font() );
	labelsize( fl_size() );
	Fl_Choice::callback( static_callback, this );
}


void Flx_Choice::static_callback( Fl_Widget *, void *pUserdata ) {
	Flx_Choice *pThis = (Flx_Choice*)pUserdata;
    ActionParm parm;
	pThis->signalSelected.send( *pThis, parm );
}

Flx_Choice::~Flx_Choice(void) {
}

} //flx
