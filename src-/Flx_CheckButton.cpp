#include <flx/Flx_CheckButton.h>
#include <flx/flx_boxtypes.h>

#include <FL/fl_draw.H>

namespace flx {

Flx_CheckButton::Flx_CheckButton( int x, int y, int w, int h, const char *pLabel )
	: Fl_Check_Button( x, y, w, h, pLabel )
{
	//box( FL_FLAT_BOX );
	//box( FLX_GRADIENT_BOX );
	labelfont( fl_font() );
	labelsize( fl_size() );
	Fl_Check_Button::callback( callback, this );
}

void Flx_CheckButton::callback( Fl_Widget *, void *pUserdata ) {
	Flx_CheckButton *pThis = (Flx_CheckButton*)pUserdata;
    ActionParm parm;
	pThis->signalCheckChanged.send( *pThis, parm );
}

Flx_CheckButton::~Flx_CheckButton(void)
{
}

} // flx
