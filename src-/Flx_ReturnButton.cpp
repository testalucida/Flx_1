#include <flx/Flx_ReturnButton.h>
#include <flx/flx_boxtypes.h>

#include <FL/fl_draw.H>
#include <FL/Fl.H>

namespace flx {

Flx_ReturnButton::Flx_ReturnButton( int x, int y, int w, int h, const char *pLabel )
    : Fl_Return_Button( x, y, w, h, pLabel )
{
    box( FLX_GRADIENT_BORDER_BOX );
	down_box( FLX_GRADIENT_BORDER_DOWN_BOX );
	labelfont( fl_font() );
	labelsize( fl_size() );
	callback( staticCallback, this );
}

int Flx_ReturnButton::handle( int evt ) {
	if( evt == FL_KEYDOWN ) {
		if( Fl::event_key() == FL_Enter && ( Fl::focus() == this ) ) {
			//Fl_Button handles Fl_Enter by changing focus.
			//We want to handle Fl_Enter like pressing the button.
			Fl_Button::simulate_key_action();
			ActionParm action;
			signalSelected.send( *this, action );
			return 1;
		}

	}
	//return rc;
	return Fl_Button::handle( evt );
}

void Flx_ReturnButton::staticCallback( Fl_Widget *, void *pUserdata ) {
	Flx_ReturnButton *pThis = (Flx_ReturnButton*)pUserdata;
	ActionParm action;
	pThis->signalSelected.send( *pThis, action );
}

Flx_ReturnButton::~Flx_ReturnButton()
{
    //dtor
}

} //flx
