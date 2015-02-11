#include <flx/Flx_MultiLineInput.h>
#include <flx/flx_signalparms.h>

#include <FL/fl_draw.H>

namespace flx {

Flx_MultiLineInput::Flx_MultiLineInput( int x, int y, int w, int h, const char *pLabel )
	: Fl_Multiline_Input( x, y, w, h, pLabel )
{
	textfont( fl_font() );
	textsize( fl_size() );
	labelfont( fl_font() );
	labelsize( fl_size() );
    callback( staticCallback, this );
    when( FL_WHEN_CHANGED );
}

void Flx_MultiLineInput::staticCallback( Fl_Widget *, void *pUserdata ) {
    Flx_MultiLineInput *pThis = (Flx_MultiLineInput*)pUserdata;
    ActionParm parm;
    pThis->signalChanged.send( pThis, &parm );
    
}

int Flx_MultiLineInput::handle( int e ) {
  switch (e) {
    case FL_PUSH:
    case FL_FOCUS:
      {
        bool focus = true;
        signalFocus.send( this, &focus );
      }
      break;
    case FL_UNFOCUS:
      {
        bool focus = false;
        signalFocus.send( this, &focus );
      }
      break;
    default:
      break;
  }
  return( Fl_Multiline_Input::handle( e ) );
}

Flx_MultiLineInput::~Flx_MultiLineInput(void)
{
}

}