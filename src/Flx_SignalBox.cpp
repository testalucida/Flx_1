#include <flx/Flx_SignalBox.h>

#include <FL/names.h>
#include <FL/Fl.H>

namespace flx {

Flx_SignalBox::Flx_SignalBox( int x, int y, int w, int h, const char *pLabel )
	: Flx_Box( x, y, w, h, pLabel ) 
{
}

int Flx_SignalBox::handle( int evt ) {
	switch( evt ) {
	case FL_ENTER: 
		{
			if( signalMouseEnter.getConnectionCount() > 0 ) {
				MouseParm mp = { Fl::event_x(), Fl::event_y() };
				signalMouseEnter.send( *this, mp );
			}
		}
		break;
	case FL_LEAVE:
		{
			if( signalMouseLeave.getConnectionCount() > 0 ) {
				MouseParm mp = { Fl::event_x(), Fl::event_y() };
				signalMouseLeave.send( *this, mp );
			}
		}
		break;
	case FL_PUSH:
		{
			if( signalClick.getConnectionCount() > 0 ) {
				ClickParm cp;
				cp.mouseBtn = Fl::event_button();
				cp.doubleClicked = ( Fl::event_clicks() != 0 ) ? true : false;
				cp.x = Fl::event_x();
				cp.y = Fl::event_y();
				signalClick.send( *this, cp );
			}
		}
		break;
	}
	return Flx_Box::handle( evt );
}

} //ns flx