#include <flx/Flx_Input.h>
#include <flx/flx_boxtypes.h>

#include <FL/fl_draw.H>
#include <FL/Fl.H>

namespace flx {

Flx_Input::Flx_Input( int x, int y, int w, int h, const char *pLabel )
	: Fl_Input( x, y, w, h, pLabel )
{
	box( FL_THIN_DOWN_BOX );
	//color( FL_WHITE );
	textfont( fl_font() );
	textsize( fl_size() );
	labelfont( fl_font() );
	labelsize( fl_size() );
	callback( staticCallback, this );
	when( FL_WHEN_ENTER_KEY_ALWAYS );
}

void Flx_Input::staticCallback( Fl_Widget *, void * pUserdata ) {
	Flx_Input *pInp = (Flx_Input*)pUserdata;
	ActionParm parm;
	pInp->signalEnterPressed.send( *pInp, parm );
}

int Flx_Input::handle( int evt ) {
	//if( evt == FL_KEYDOWN ) {
	//	if( Fl::event_key() == FL_Enter && ( Fl::focus() == this ) ) {
	//		//Fl_Input handles Fl_Enter by changing focus.
	//		//We want to handle Fl_Enter like pressing the focused button (if any).
	//		//ActionParm action;
	//		//signalSelected.send( *this, action );
	//		return 1;
	//	}
	//	
	//}
	//return rc;
	return Fl_Input::handle( evt );
}

Flx_Input::~Flx_Input(void)
{
}

} //flx
