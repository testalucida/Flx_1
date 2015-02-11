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
//	callback( staticCallback, this );
//    when( FL_WHEN_NOT_CHANGED );
//	when( FL_WHEN_ENTER_KEY_ALWAYS );
}

/** for compatibility only */
//void Flx_Input::staticCallback( Fl_Widget *, void * pUserdata ) {
//	Flx_Input *pInp = (Flx_Input*)pUserdata;
//	ActionParm parm;
//	//pInp->signalEnterPressed.send( *pInp, parm );
//    pInp->signalChanged.send( *pInp, parm );
//}

int Flx_Input::handle( int evt ) {
   
    int rc =  Fl_Input::handle( evt );
    
    switch( evt ) {
        case FL_KEYDOWN:
            if( Fl::focus() == this ) {
                ActionParm parm;
                if( Fl::event_key() == FL_Enter ) {
                    signalEnterPressed.send( *this, parm );
                } else if( Fl::event_key() == FL_Tab ) {
                    signalTabPressed.send( *this, parm );
                } else {
                    signalChanged.send( *this, parm );
                }
            }
            break;
        case FL_PASTE:
            ActionParm parm;
            signalChanged.send( *this, parm );
            break;
        case FL_PUSH:
        {
            int len = strlen( this->value() );
            position( 0, len );
            break;
        }
        case FL_FOCUS:
        {
            int len = strlen( this->value() );
            position( 0, len );
            break;
        }
        default:
            break;
    }
 
	return rc;
}

void Flx_Input::value( const char *pVal ) {
    Fl_Input::value( pVal );
    ActionParm parm;
    signalChanged.send( *this, parm );
}

Flx_Input::~Flx_Input(void)
{
}

} //flx
