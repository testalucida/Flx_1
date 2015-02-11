#include <flx/Flx_Window.h>
#include <flx/flx_boxtypes.h>

#include <FL/Fl.H>
#include <FL/names.h>
#include <FL/fl_draw.H>

#include <stdio.h>

namespace flx {


Flx_Window::Flx_Window( int x, int y, int w, int h, const char *pLabel )
	: Fl_Double_Window( x, y, w, h, pLabel )
	, _shortCutComing( false )
{
	Fl::scheme( "GTK+" );
	Flx_BoxDrawing::init();
	box( FL_FLAT_BOX );
	color( fl_color_average( FL_WHITE, FL_GRAY, .5f ) );
	fl_font( FL_HELVETICA, 12 );
}

int Flx_Window::handle( int evt ) {
	//printf( "Flx_Window::handle: evt = %s (%d)\n", fl_eventnames[evt], evt );
	//switch( evt ) {
	//case FL_SHORTCUT:
	//	printf( "Flx_Window::handle( FL_SHORTCUT): evt = %s (%d)\n", fl_eventnames[evt], evt );
	//	_shortCutComing = true;
	//	return 1;
	//	break;
	//case FL_KEYDOWN:
	//	if( _shortCutComing ) {
	//		printf( "Flx_Window::handle(FL_KEYDOWN): evt = %s (%d), event_key=%d\n", 
	//				fl_eventnames[evt], evt, Fl::event_key() );
	//		if( Fl::event_key() == 'm' )
	//			printf( "M!!\n" );
	//		_shortCutComing = false;
	//		return 1;
	//	}
	//	break;
	//default:
	//	break;	
	//}
	return Fl_Double_Window::handle( evt );
}

} //ns flx