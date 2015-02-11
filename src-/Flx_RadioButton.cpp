
#include <flx/Flx_RadioButton.h>

#include <FL/fl_draw.H>
#include <FL/Fl.H>

namespace flx {

Flx_RadioButton::Flx_RadioButton( int x, int y, int w, int h, const char *pLabel )
	: Fl_Round_Button( x, y, w, h, pLabel )
{
	labelfont( fl_font() );
	labelsize( fl_size() );
	callback( staticCallback, this );
}

void Flx_RadioButton::staticCallback( Fl_Widget *, void *pUserdata ) {
	Flx_RadioButton *pThis = (Flx_RadioButton*)pUserdata;
	ActionParm action;
	pThis->signalSelected.send( *pThis, action );
}


Flx_RadioButton::~Flx_RadioButton(void) {
}

}
